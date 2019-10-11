#ifndef RESIZING_KERNELS_DB
#define RESIZING_KERNELS_DB
#include  "apu2_kernel_db_lib.h"

static const KERNEL_INFO resizing_kernels_db_0(
	"apu_downsample", 2, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	2)
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
static const KERNEL_INFO resizing_kernels_db_1(
	"apu_downsample_16u", 2, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	2)
	),
	__port(
		__index(1),
		__identifier("OUTPUT_0"),
		__attributes(ATTR_OUT|ATTR_CMEM),
		__spatial_dep(	0, 	0, 	0, 	0),
		__e0_data_type(d16u),
		__e0_size(	1, 	1),
		__ek_size(	1, 	1)
	)
);
static const KERNEL_INFO resizing_kernels_db_2(
	"apu_downsample_gauss", 2, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	1, 	1, 	1, 	1),
		__e0_data_type(d08u),
		__e0_size(	1, 	1),
		__ek_size(	2, 	2)
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
static const KERNEL_INFO resizing_kernels_db_3(
	"apu_upsample", 2, 
	__port(
		__index(0),
		__identifier("INPUT_0"),
		__attributes(ATTR_IN|ATTR_CMEM),
		__spatial_dep(	1, 	1, 	1, 	1),
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
		__ek_size(	2, 	2)
	)
);
#endif // RESIZING_KERNELS_DB
