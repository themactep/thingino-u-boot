#include <errno.h>
#include <malloc.h>
#include <linux/mtd/partitions.h>
#include "../jz_sfc_nand.h"
#include "nand_common.h"

#define WODPOSIT_DEVICES_NUM         1
#define TSETUP		40
#define THOLD		40
#define	TSHSL_R		20
#define	TSHSL_W		50

static struct jz_nand_base_param wodposit_param[WODPOSIT_DEVICES_NUM] = {

	[0] = {
		/*WPS3NS01W-S4YO*/
		.pagesize = 2 * 1024,
		.blocksize = 2 * 1024 * 64,
		.oobsize = 64,
		.flashsize = 2 * 1024 * 64 * 1024,

		.tSETUP  = TSETUP,
		.tHOLD   = THOLD,
		.tSHSL_R = TSHSL_R,
		.tSHSL_W = TSHSL_W,

		.ecc_max = 0x6,
#ifdef CONFIG_SPI_STANDARD
		.need_quad = 0,
#else
		.need_quad = 1,
#endif
	},

};

static struct device_id_struct device_id[WODPOSIT_DEVICES_NUM] = {
	DEVICE_ID_STRUCT(0xA0, "WPS3NS01W", &wodposit_param[0]),
};

static int32_t wodposit_get_read_feature(struct flash_operation_message *op_info) {

	struct sfc_flash *flash = op_info->flash;
	struct jz_nand_descriptor *nand_desc = flash->flash_info;
	struct sfc_transfer transfer;
	struct sfc_message message;
	struct cmd_info cmd;
	uint8_t device_id = nand_desc->id_device;
	uint8_t ecc_status = 0;
	int32_t ret = 0;

	memset(&transfer, 0, sizeof(transfer));
	memset(&cmd, 0, sizeof(cmd));
	sfc_message_init(&message);

	cmd.cmd = SPINAND_CMD_GET_FEATURE;
	transfer.sfc_mode = TM_STD_SPI;

	transfer.addr = SPINAND_ADDR_STATUS;
	transfer.addr_len = 1;

	cmd.dataen = DISABLE;
	transfer.len = 0;

	transfer.data_dummy_bits = 0;
	cmd.sta_exp = (0 << 0);
	cmd.sta_msk = SPINAND_IS_BUSY;
	transfer.cmd_info = &cmd;
	transfer.ops_mode = CPU_OPS;

	sfc_message_add_tail(&transfer, &message);
	if(sfc_sync(flash->sfc, &message)) {
	        printf("sfc_sync error ! %s %s %d\n",__FILE__,__func__,__LINE__);
		return -EIO;
	}

	ecc_status = sfc_get_sta_rt(flash->sfc);

	switch(device_id) {
		case 0xa0:
			switch((ecc_status >> 4) & 0x3) {
				case 0x00:
					ret = 0;
					break;
				case 0x01:
					ret = 4;
					break;
			    case 0x02:
				    ret = -EBADMSG;
				    break;
			    case 0x03:
				    ret = 0x5;
				    break;
			    default:
				    ret = 0;
			}
			break;
		default:
			printf("device_id err, it maybe don`t support this device, check your device id: device_id = 0x%02x\n", device_id);
			ret = -EIO;   //notice!!!

	}
	return ret;
}

int wodposit_nand_init(void) {
	struct jz_nand_device *wodposit_nand;
	wodposit_nand = kzalloc(sizeof(*wodposit_nand), GFP_KERNEL);
	if(!wodposit_nand) {
		pr_err("alloc wodposit_nand struct fail\n");
		return -ENOMEM;
	}

	wodposit_nand->id_manufactory = 0xA5;
	wodposit_nand->id_device_list = device_id;
	wodposit_nand->id_device_count = WODPOSIT_DEVICES_NUM;

	wodposit_nand->ops.nand_read_ops.get_feature = wodposit_get_read_feature;
	return jz_spinand_register(wodposit_nand);
}
SPINAND_MOUDLE_INIT(wodposit_nand_init);
