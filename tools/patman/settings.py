# Copyright (c) 2011 The Chromium OS Authors.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# SPDX-License-Identifier:	GPL-2.0+

import ConfigParser
import os
import re

import command
import gitutil

"""Default settings per-project.

These are used by _ProjectConfigParser.  Settings names should match
the "dest" of the option parser from patman.py.
"""
_default_settings = {
    "u-boot": {},
    "linux": {
        "process_tags": "False",
    }
}

class _ProjectConfigParser(ConfigParser.SafeConfigParser):
    """ConfigParser that handles projects.

    There are two main goals of this class:
    - Load project-specific default settings.
    - Merge general default settings/aliases with project-specific ones.

    # Sample config used for tests below...
    >>> import StringIO
    >>> sample_config = '''
    ... [alias]
    ... me: Peter P. <likesspiders@example.com>
    ... enemies: Evil <evil@example.com>
    ...
    ... [sm_alias]
    ... enemies: Green G. <ugly@example.com>
    ...
    ... [sm2_alias]
    ... enemies: Doc O. <pus@example.com>
    ...
    ... [settings]
    ... am_hero: True
    ... '''

    # Check to make sure that bogus project gets general alias.
    >>> config = _ProjectConfigParser("zzz")
    >>> config.readfp(StringIO.StringIO(sample_config))
    >>> config.get("alias", "enemies")
    'Evil <evil@example.com>'

    # Check to make sure that alias gets overridden by project.
    >>> config = _ProjectConfigParser("sm")
    >>> config.readfp(StringIO.StringIO(sample_config))
    >>> config.get("alias", "enemies")
    'Green G. <ugly@example.com>'

    # Check to make sure that settings get merged with project.
    >>> config = _ProjectConfigParser("linux")
    >>> config.readfp(StringIO.StringIO(sample_config))
    >>> sorted(config.items("settings"))
    [('am_hero', 'True'), ('process_tags', 'False')]

    # Check to make sure that settings works with unknown project.
    >>> config = _ProjectConfigParser("unknown")
    >>> config.readfp(StringIO.StringIO(sample_config))
    >>> sorted(config.items("settings"))
    [('am_hero', 'True')]
    """
    def __init__(self, project_name):
        """Construct _ProjectConfigParser.

        In addition to standard SafeConfigParser initialization, this also loads
        project defaults.

        Args:
            project_name: The name of the project.
        """
        self._project_name = project_name
        ConfigParser.SafeConfigParser.__init__(self)

        # Update the project settings in the config based on
        # the _default_settings global.
        project_settings = "%s_settings" % project_name
        if not self.has_section(project_settings):
            self.add_section(project_settings)
        project_defaults = _default_settings.get(project_name, {})
        for setting_name, setting_value in project_defaults.iteritems():
            self.set(project_settings, setting_name, setting_value)

    def get(self, section, option, *args, **kwargs):
        """Extend SafeConfigParser to try project_section before section.

        Args:
            See SafeConfigParser.
        Returns:
            See SafeConfigParser.
        """
        try:
            return ConfigParser.SafeConfigParser.get(
                self, "%s_%s" % (self._project_name, section), option,
                *args, **kwargs
            )
        except (ConfigParser.NoSectionError, ConfigParser.NoOptionError):
            return ConfigParser.SafeConfigParser.get(
                self, section, option, *args, **kwargs
            )

    def items(self, section, *args, **kwargs):
        """Extend SafeConfigParser to add project_section to section.

        Args:
            See SafeConfigParser.
        Returns:
            See SafeConfigParser.
        """
        project_items = []
        has_project_section = False
        top_items = []

        # Get items from the project section
        try:
            project_items = ConfigParser.SafeConfigParser.items(
                self, "%s_%s" % (self._project_name, section), *args, **kwargs
            )
            has_project_section = True
        except ConfigParser.NoSectionError:
            pass

        # Get top-level items
        try:
            top_items = ConfigParser.SafeConfigParser.items(
                self, section, *args, **kwargs
            )
        except ConfigParser.NoSectionError:
            # If neither section exists raise the error on...
            if not has_project_section:
                raise

        item_dict = dict(top_items)
        item_dict.update(project_items)
        return item_dict.items()

def ReadGitAliases(fname):
    """Read a git alias file. This is in the form used by git:

    alias uboot  u-boot@lists.denx.de
    alias wd     Wolfgang Denk <wd@denx.de>

    Args:
        fname: Filename to read
    """
    try:
        fd = open(fname, 'r')
    except IOError:
        print "Warning: Cannot find alias file '%s'" % fname
        return

    re_line = re.compile('alias\s+(\S+)\s+(.*)')
    for line in fd.readlines():
        line = line.strip()
        if not line or line[0] == '#':
            continue

        m = re_line.match(line)
        if not m:
            print "Warning: Alias file line '%s' not understood" % line
            continue

        list = alias.get(m.group(1), [])
        for item in m.group(2).split(','):
            item = item.strip()
            if item:
                list.append(item)
        alias[m.group(1)] = list

    fd.close()

def CreatePatmanConfigFile(config_fname):
    """Creates a config file under $(HOME)/.patman if it can't find one.

    Args:
        config_fname: Default config filename i.e., $(HOME)/.patman

    Returns:
        None
    """
    name = gitutil.GetDefaultUserName()
    if name == None:
        name = raw_input("Enter name: ")

    email = gitutil.GetDefaultUserEmail()

    if email == None:
        email = raw_input("Enter email: ")

    try:
        f = open(config_fname, 'w')
    except IOError:
        print "Couldn't create patman config file\n"
        raise

    print >>f, "[alias]\nme: %s <%s>" % (name, email)
    f.close();

def _UpdateDefaults(parser, config):
    """Update the given OptionParser defaults based on config.

    We'll walk through all of the settings from the parser
    For each setting we'll look for a default in the option parser.
    If it's found we'll update the option parser default.

    The idea here is that the .patman file should be able to update
    defaults but that command line flags should still have the final
    say.

    Args:
        parser: An instance of an OptionParser whose defaults will be
            updated.
        config: An instance of _ProjectConfigParser that we will query
            for settings.
    """
    defaults = parser.get_default_values()
    for name, val in config.items('settings'):
        if hasattr(defaults, name):
            default_val = getattr(defaults, name)
            if isinstance(default_val, bool):
                val = config.getboolean('settings', name)
            elif isinstance(default_val, int):
                val = config.getint('settings', name)
            parser.set_default(name, val)
        else:
            print "WARNING: Unknown setting %s" % name

def Setup(parser, project_name, config_fname=''):
    """Set up the settings module by reading config files.

    Args:
        parser:         The parser to update
        project_name:   Name of project that we're working on; we'll look
            for sections named "project_section" as well.
        config_fname:   Config filename to read ('' for default)
    """
    config = _ProjectConfigParser(project_name)
    if config_fname == '':
        config_fname = '%s/.patman' % os.getenv('HOME')

    if not os.path.exists(config_fname):
        print "No config file found ~/.patman\nCreating one...\n"
        CreatePatmanConfigFile(config_fname)

    config.read(config_fname)

    for name, value in config.items('alias'):
        alias[name] = value.split(',')

    _UpdateDefaults(parser, config)

# These are the aliases we understand, indexed by alias. Each member is a list.
alias = {}

if __name__ == "__main__":
    import doctest

    doctest.testmod()
