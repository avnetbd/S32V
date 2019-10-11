#ifndef APEXCV_BASE_ARITHMETIC_DB
#define APEXCV_BASE_ARITHMETIC_DB
#include  "apu2_kernel_db_lib.h"

static const KERNEL_INFO apexcv_base_arithmetic_db_0(
	"abs_08u", 2, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_1(
	"absdiff_08u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_2(
	"absdiff_16s", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_3(
	"accumulate_16s", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_4(
	"accumulate_squared_16s", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("ALPHA"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_5(
	"accumulate_weighted_08u", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("ALPHA"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_6(
	"add_08u", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_7(
	"add_08u_08u_16s", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_8(
	"add_08u_16s_16s", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_9(
	"add_16s", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_10(
	"and_08u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_11(
	"and_16u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_12(
	"and_32u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_13(
	"clz_08u", 2, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_14(
	"clz_08s", 2, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_15(
	"clz_16u", 2, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_16(
	"clz_16s", 2, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_17(
	"magnitude_16s", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_18(
	"max_08u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_19(
	"max_16s", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_20(
	"min_08u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_21(
	"min_16s", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_22(
	"mul_08u", 5, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("INPUT_3"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(4),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_23(
	"mul_08u_08u_16s", 5, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("INPUT_3"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(4),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_24(
	"mul_08u_16s_16s", 5, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("INPUT_3"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(4),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_25(
	"mul_16s", 5, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("INPUT_3"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(4),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_26(
	"not_08u", 2, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_27(
	"or_08u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_28(
	"or_16u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_29(
	"or_32u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_30(
	"sub_08u", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_31(
	"sub_08u_08u_16s", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_32(
	"sub_08u_16s_16s", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_33(
	"sub_16s", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_34(
	"sub_16s_08u_16s", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16s),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_35(
	"table_lookup_08u", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	256, 	1)
	),
	__port(
		__index(3),
		__identifier("VEC_LUT"),
		__attributes(ATTR_OUT|ATTR_CMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	256, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_36(
	"threshold_08u", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_37(
	"threshold_16u", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_38(
	"threshold_32u", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_39(
	"threshold_range_08u", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_40(
	"threshold_range_16u", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_41(
	"threshold_range_32u", 4, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	1)
	),
	__port(
		__index(2),
		__identifier("INPUT_2"),
		__attributes(ATTR_IN|ATTR_DMEM|ATTR_STATIC|ATTR_FIXED),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	1)
	),
	__port(
		__index(3),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_42(
	"xor_08u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_43(
	"xor_16u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO apexcv_base_arithmetic_db_44(
	"xor_32u", 3, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(1),
		__identifier("INPUT_1"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	),
	__port(
		__index(2),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d32u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
#endif // APEXCV_BASE_ARITHMETIC_DB
