//
// Created by hujianzhe on 18-8-17.
//

#ifndef	UTIL_C_COMPONENT_LENGTHFIELDFRAME_H
#define	UTIL_C_COMPONENT_LENGTHFIELDFRAME_H

#ifdef __cplusplus
extern "C" {
#endif

__declspec_dll int lengthfieldframeDecode(unsigned short lengthfieldsize,
		unsigned char* buf, unsigned int len, unsigned char** data, unsigned int* datalen);
__declspec_dll int lengthfieldframeEncode(void* lengthfieldbuf, unsigned short lengthfieldsize, unsigned int datalen);

#ifdef __cplusplus
}
#endif

#endif
