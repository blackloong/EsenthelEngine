#define _GNU_SOURCE

#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h> 

#include "qcms.h"

const unsigned char gray_icc[] =
{0x00, 0x00, 0x01, 0x70, 0x4E, 0x4B, 0x4F, 0x4E, 0x02, 0x20, 0x00, 0x00,
0x6D, 0x6E, 0x74, 0x72, 0x47, 0x52, 0x41, 0x59, 0x58, 0x59, 0x5A, 0x20,
0x07, 0xCE, 0x00, 0x0B, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x61, 0x63, 0x73, 0x70, 0x41, 0x50, 0x50, 0x4C, 0x00, 0x00, 0x00, 0x00,
0x6E, 0x6F, 0x6E, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF6, 0xD6,
0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xD3, 0x2D, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
0x64, 0x65, 0x73, 0x63, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x4B,
0x77, 0x74, 0x70, 0x74, 0x00, 0x00, 0x01, 0x0C, 0x00, 0x00, 0x00, 0x14,
0x6B, 0x54, 0x52, 0x43, 0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 0x00, 0x0E,
0x63, 0x70, 0x72, 0x74, 0x00, 0x00, 0x01, 0x30, 0x00, 0x00, 0x00, 0x2C,
0x62, 0x6B, 0x70, 0x74, 0x00, 0x00, 0x01, 0x5C, 0x00, 0x00, 0x00, 0x14,
0x64, 0x65, 0x73, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A,
0x4E, 0x69, 0x6B, 0x6F, 0x6E, 0x47, 0x72, 0x61, 0x79, 0x47, 0x31, 0x2E,
0x38, 0x20, 0x76, 0x34, 0x2E, 0x30, 0x2E, 0x30, 0x2E, 0x33, 0x30, 0x30,
0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x1A, 0x4E, 0x69, 0x6B, 0x6F, 0x6E, 0x47, 0x72, 0x61, 0x79, 0x47, 0x31,
0x2E, 0x38, 0x20, 0x76, 0x34, 0x2E, 0x30, 0x2E, 0x30, 0x2E, 0x33, 0x30,
0x30, 0x30, 0x00, 0x00, 0x58, 0x59, 0x5A, 0x20, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xF3, 0x51, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x16, 0xCC,
0x63, 0x75, 0x72, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
0x01, 0xCD, 0x00, 0x00, 0x74, 0x65, 0x78, 0x74, 0x00, 0x00, 0x00, 0x00,
0x4E, 0x69, 0x6B, 0x6F, 0x6E, 0x20, 0x49, 0x6E, 0x63, 0x2E, 0x20, 0x26,
0x20, 0x4E, 0x69, 0x6B, 0x6F, 0x6E, 0x20, 0x43, 0x6F, 0x72, 0x70, 0x6F,
0x72, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x32, 0x30, 0x30, 0x31, 0x00,
0x58, 0x59, 0x5A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void test_gray(qcms_profile *output_profile)
{
	unsigned char srct[3] = { 221, 79, 129};
	unsigned char outt[3];
	qcms_transform *transform;
	qcms_profile *gray_profile = qcms_profile_from_memory(gray_icc, sizeof(gray_icc));
	if (gray_profile) {
		transform = qcms_transform_create(gray_profile, QCMS_DATA_GRAY_8, output_profile, QCMS_DATA_RGB_8, QCMS_INTENT_PERCEPTUAL);
		if (transform) {
			qcms_transform_data(transform, srct, outt, 1);
			qcms_transform_release(transform);
		}
		qcms_profile_release(gray_profile);
	}

}

void test_gray_precache(qcms_profile *output_profile)
{
	unsigned char srct[3] = { 221, 79, 129};
	unsigned char outt[3];
	qcms_transform *transform;
	qcms_profile *gray_profile = qcms_profile_from_memory(gray_icc, sizeof(gray_icc));
	qcms_profile_precache_output_transform(output_profile);
	if (gray_profile) {
		transform = qcms_transform_create(gray_profile, QCMS_DATA_GRAY_8, output_profile, QCMS_DATA_RGB_8, QCMS_INTENT_PERCEPTUAL);
		if (transform) {
			qcms_transform_data(transform, srct, outt, 1);
			qcms_transform_release(transform);
		}
		qcms_profile_release(gray_profile);
	}

}
void do_test(void) {
	unsigned char srct[4] = { 221, 79, 129, 92};
	unsigned char outt[4];
	qcms_transform *transform;
	qcms_profile *input_profile, *output_profile, *rgb;
	qcms_CIE_xyY white_point = { 0, 0, 1.};
	qcms_CIE_xyYTRIPLE primaries = { {.9, .3, 1.}, {.2, .4, 1.}, {.7, .4, 1.}};

	rgb = qcms_profile_create_rgb_with_gamma(white_point, primaries, 1.8);
	input_profile = qcms_profile_sRGB();
	output_profile = qcms_profile_sRGB();
	if (output_profile) {
		test_gray(output_profile);
		test_gray_precache(output_profile);
		qcms_profile_release(output_profile);
	}
	if (input_profile)
		qcms_profile_release(input_profile);
	if (rgb)
		qcms_profile_release(rgb);
	rgb = qcms_profile_from_path("sample-trunc.icc");
	if (rgb)
		qcms_profile_release(rgb);
}

static int current_counter;
static int counter;

void* malloc(size_t size)
{
	if (counter == current_counter++) {
		return NULL;
	}
	static void* (*real_malloc)(size_t size);
	if (!real_malloc)
		real_malloc = (void* (*)(size_t)) dlsym(RTLD_NEXT, "malloc");
	return real_malloc(size);
}

void* calloc(size_t count, size_t size)
{
	if (counter == current_counter++) {
		return NULL;
	}
	static void* (*real_calloc)(size_t count, size_t size);
	if (!real_calloc)
		real_calloc = (void* (*)(size_t, size_t)) dlsym(RTLD_NEXT, "calloc");
	return real_calloc(count, size);
}


const extern unsigned char gray_icc[];
int main()
{
	while (current_counter >= counter) {
		current_counter = 0;
		do_test();
		counter++;
	}
	return 0;
}
