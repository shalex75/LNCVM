#ifndef LN_CV_USER
#define LN_CV_USER

#include <cv_init.h>

#define LNCV_COUNT 7

#define LNC_CV_EXT_READ 10

const  CVDesc CV_USER_INIT[LNCV_COUNT] PROGMEM ={
    {LNC_CV_EXT_READ,     1,  CV_RW_EXT, "User 1"},
    {11,     2,  CV_RW, "User 2"},
    {12,     2,  CV_RW, "User 3"},
    {13,     2,  CV_RW, "User 4"},
    {14,     2,  CV_RW, "User 5"},
    {15,     2,  CV_RW, "User 6"},
    {16,     2,  CV_RW, "User 7"},
};
#endif