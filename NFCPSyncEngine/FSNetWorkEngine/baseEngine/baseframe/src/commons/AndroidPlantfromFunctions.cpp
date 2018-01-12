/*
 * AndroidPlantfromFunctions.cpp
 *
 *  Created on: 2014-12-6
 *      Author: zhouhewei
 */


//
//  CNetFactory.cpp
//  commons
//
//  Created by �ܺ�ΰ on 14/12/3.
//  Copyright (c) 2014�� �ܺ�ΰ. All rights reserved.
//

#include <stdio.h>
#include "CNetFactory.h"
#include "CFileFactory.h"
#include "CImageFunctions.h"
#include "native_log.h"
#include "CShareMemManager.h"

#ifdef DEBUG
#include <android/log.h>
#endif

#ifdef USE_LIB_HTTP
#include "ghttplib/ghttp.h"
#endif

#ifdef USE_LIB_JPEG
#include "jpeg-turbo/inc/jpeglib.h"
#endif
 
extern "C"{
	_HANDLE Call_Java_HTTP_GET( _string& url );
	_HANDLE Call_Java_FILE_GET( _string& uri );
	_BOOL   Call_Java_IMAGE_FUNCTION( _PVOID buffer, _UINT bufferSize, _HANDLE& PixelBufferHandler, _UINT& width, _UINT& height );
	_BOOL   Call_Java_FONT_FUCTION( _string& text, _FLOAT fontSize, _CONST _ULONG frontColor, _CONST _ULONG backColor, _CONST _ULONG padding, _HANDLE& PixelBufferHandler, _UINT& width, _UINT& height );
}
_HANDLE CNetFactory::HTTP_GET( _string url )
{

#ifndef USE_LIB_HTTP
    return Call_Java_HTTP_GET (url );
#else
    int tryTime = 3;
    ghttp_request *request = NULL;
    ghttp_status status;
    
    _HANDLE handler = 0;
    while( tryTime-- )
    {
        request = ghttp_request_new();
        
        if( ghttp_set_uri(request, (char *) url.c_str()) == -1)
            continue;
        /* Close the connection after you are done. */
        if( ghttp_set_type(request, ghttp_type_get) == -1 )
            continue;
        
        ghttp_set_header(request, http_hdr_Connection, "close");
        ghttp_prepare(request);
        status = ghttp_process(request);
        
        if( status == ghttp_error )
            continue;
        
        if( ghttp_status_code(request) == 200 )
        {
            char* olddata = ghttp_get_body(request);
            _UINT oldsize = ghttp_get_body_len(request);
            handler = ShareMemManager->createSharedMemory(oldsize);
      			ICSharedMemory* sm = ShareMemManager->getObject(handler);
      			_PVOID buffer = sm->obtainMemoryLock();
      			memcpy(buffer, olddata, oldsize);
      			sm->releaseMemoryLock();

            ghttp_request_destroy(request);
            break;
        }
        ghttp_request_destroy(request);
    }

    return handler;
#endif
}


_HANDLE  CFileFactory::FILE_GET( _string url )
{
    return Call_Java_FILE_GET( url );
}

_BOOL getFontPixelBuffer( _string& text, _FLOAT fontSize,_CONST _ULONG frontColor, _CONST _ULONG backColor, _CONST _ULONG padding, _HANDLE& PixelBufferHandler, _UINT& width, _UINT& height )
{
	return Call_Java_FONT_FUCTION( text, fontSize, frontColor, backColor, padding, PixelBufferHandler, width, height );
}

// _BOOL getImagePixelBuffer( _PVOID buffer, _ULONG bufferSize, _HANDLE& PixelBufferHandler, _UINT& width, _UINT& height )
// {
//     return Call_Java_IMAGE_FUNCTION( buffer, bufferSize, PixelBufferHandler, width, height );
// }

//ImageFunctions

//#define  readBuffer( dest, src, size )  \
//do\
//{\
//_PCHAR buf = (_PCHAR)src;\
//memcpy( dest, buf, (size) );\
//buf += size;\
//src = buf;\
//}while(0);
//
/////bmp headers
//typedef struct tagBITMAPFILEHEADER {
//    unsigned char bfType[2];
//    unsigned char bfSize[4];
//    unsigned char bfReserved1[2];
//    unsigned char bfReserved2[2];
//    unsigned char bfOffBits[4];
//} BITMAPFILEHEADER;
//
//typedef struct tagBITMAPINFOHEADER {
//    unsigned char biSize[4];
//    unsigned char biWidth[4];
//    unsigned char biHeight[4];
//    unsigned char biPlanes[2];
//    unsigned char biBitCount[2];
//    unsigned char biCompression[4];
//    unsigned char biSizeImage[4];
//    unsigned char biXPelsPerMeter[4];
//    unsigned char biYPelsPerMeter[4];
//    unsigned char biClrUsed[4];
//    unsigned char biClrImportant[4];
//} BITMAPINFOHEADER;
/////////////////
//
//int dword_to_long(unsigned char * dwordPtr) {
//    int* result = (int*)dwordPtr;
//    return *result;
//}
//
//
//
//_BOOL BmpImage( _PVOID buffer, _HANDLE& PixelBufferHandler, _UINT& width, _UINT& height )
//{
//    unsigned char tmpRGB;
//    BITMAPFILEHEADER fileHeader;
//    BITMAPINFOHEADER infoHeader;
//
//    _PVOID _temp = buffer;
//
//    readBuffer( &fileHeader, _temp, sizeof(BITMAPFILEHEADER) );
//    readBuffer( &infoHeader, _temp, sizeof(BITMAPINFOHEADER) );
//
//    width   = dword_to_long(infoHeader.biWidth);
//    height  = dword_to_long(infoHeader.biHeight);
//
//    if ((infoHeader.biBitCount[0]) != 0x18) {
//        printf("���?��24λͼ!\r\n");
//        return _FALSE;
//    }
//
//    PixelBufferHandler = ShareMemManager->createSharedMemory( *(_PINT)infoHeader.biSizeImage );
//    ICSharedMemory* sm = ShareMemManager->getObject( PixelBufferHandler );
//    _PCHAR ptr = (_PCHAR)sm->obtainMemoryLock();
//
//    _UINT  dataSize = *(_PINT)fileHeader.bfOffBits;
//    _PVOID imageData = (_PCHAR)buffer + dataSize;
//
//    readBuffer( ptr, imageData, dataSize );
//
//    for (unsigned int i = 0; i < *(int*)infoHeader.biSizeImage; i+=3)
//    {
//        tmpRGB = ptr[i];
//        ptr[i] = ptr[i+2];
//        ptr[i+2] = tmpRGB;
//    }
//    sm->releaseMemoryLock();
//    return _TRUE;
//}
//
////jpeg
//
//struct my_error_mgr
//{
//    struct jpeg_error_mgr pub;
//    jmp_buf setjmp_buffer;
//};
//
//typedef struct my_error_mgr * my_error_ptr;
//
//METHODDEF(void) my_error_exit (j_common_ptr cinfo)
//{
//    my_error_ptr myerr = (my_error_ptr) cinfo->err;
//    (*cinfo->err->output_message) (cinfo);
//    longjmp(myerr->setjmp_buffer, 1);
//}
//
//

#ifdef USE_LIB_JPEG
_BOOL JpegImage( _PVOID inBuffer, _UINT bufferSize,_HANDLE& PixelBufferHandler, _UINT& width, _UINT& height )
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    
    _INT row_stride;
    
    if( inBuffer == _NULL )
    {
        return _FALSE;
    }
    
    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(&jerr);
    
    

    jpeg_mem_src( &cinfo, (_PUCHAR)inBuffer, bufferSize );
    jpeg_read_header(&cinfo, TRUE);
    
//    cinfo.scale_num = 1;
//    cinfo.scale_denom = 8;
    jpeg_start_decompress(&cinfo);
    
    row_stride = cinfo.output_width * cinfo.output_components;
    
    _CHAR alignment     = 4;
    _CHAR complement    = row_stride % alignment;
    if ( complement != 0 )
    {
        row_stride += alignment - complement;
    }
    
    
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    
    width   = cinfo.output_width;
    height  = cinfo.output_height;
    PixelBufferHandler = ShareMemManager->createSharedMemory( row_stride * cinfo.output_height );
    
    _ULONG  size = row_stride * cinfo.output_height;
    _CHAR* pbuff = (char*)malloc( size );
    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(&pbuff[ (cinfo.output_scanline - 1) * row_stride], buffer[0], row_stride );
    }
    
    ICSharedMemory* sm = ShareMemManager->getObject( PixelBufferHandler );
    _CHAR* ptr = (_CHAR*)sm->obtainMemoryLock();
    memcpy( ptr, pbuff, size );
    sm->releaseMemoryLock();
    SAFE_FREE( pbuff );

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    return _TRUE;
}
#endif
//png

//typedef struct
//{
//    _PUCHAR data;
//    _INT    size;
//    _INT    offset;
//}ImageSource;
//
//static _VOID pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
//{
//    ImageSource* isource = (ImageSource*)png_get_io_ptr(png_ptr);
//    if(isource->offset + length <= isource->size)
//    {
//        memcpy(data, isource->data+isource->offset, length);
//        isource->offset += length;
//    }
//    else
//        png_error(png_ptr, "pngReaderCallback failed");
//}

//_BOOL PngImage( _PVOID inBuffer, _UINT bufferSize,_HANDLE& PixelBufferHandler, _UINT& width, _UINT& height )
//{

//    _CHAR   header[8];
//    png_byte color_type;            //ͼƬ�����ͣ����ܻ������Ƿ��ǿ�����ͨ����
//    png_byte bit_depth;             //�ֽ����
//    png_structp png_ptr;            //ͼƬ
//    png_infop info_ptr;              //ͼƬ����Ϣ
//    png_bytep * row_pointers;   //ͼƬ���������
//
//    _PUCHAR rgba;
//    _PUCHAR ptr = _NULL;
//
//
//    if( inBuffer == _NULL )
//        return _FALSE;
//
//
//    //�ж��Ƿ���pngͼƬ.������������Ӧ����
//    memcpy(header , ptr, 8 );
//    if(png_sig_cmp( (png_bytep)header, 0, 8))
//    {
//        return _FALSE;
//    }
//
//    png_ptr=png_create_read_struct(PNG_LIBPNG_VER_STRING, _NULL, _NULL, _NULL);
//
//    if(!png_ptr)
//    {
//        return _FALSE;
//    }
//
//    info_ptr=png_create_info_struct(png_ptr);
//
//    if(!info_ptr)
//    {
//        png_destroy_read_struct(&png_ptr,(png_infopp)_NULL,(png_infopp)_NULL);
//        return _FALSE;
//    }
//
//    if (setjmp(png_jmpbuf(png_ptr)))
//    {
//        png_destroy_read_struct(&png_ptr,(png_infopp)_NULL,(png_infopp)_NULL);
//        return _FALSE;
//    }
//
//
//    ImageSource imgsource;
//    imgsource.data = (_PUCHAR)inBuffer;
//    imgsource.size = bufferSize;
//    imgsource.offset = 0;
//
//    png_set_read_fn(png_ptr, &imgsource, pngReadCallback);
//
//    png_read_info(png_ptr, info_ptr);
//    width       = info_ptr->width;
//    height      = info_ptr->height;
//    color_type  = info_ptr->color_type;
//    bit_depth   = info_ptr->bit_depth;
//
//    // Convert stuff to appropriate formats!
//    if(color_type==PNG_COLOR_TYPE_PALETTE)
//    {
//        png_set_packing(png_ptr);
//        png_set_palette_to_rgb(png_ptr); //Expand data to 24-bit RGB or 32-bit RGBA if alpha available.
//
//    }
//    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
//        png_set_expand_gray_1_2_4_to_8(png_ptr);
//    if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
//        png_set_gray_to_rgb(png_ptr);
//    if (bit_depth == 16)
//        png_set_strip_16(png_ptr);
//
//
//    //Expand paletted or RGB images with transparency to full alpha channels so the data will be available as RGBA quartets.
//    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
//    {
//        png_set_tRNS_to_alpha(png_ptr);
//    }
//
//    PixelBufferHandler = ShareMemManager->createSharedMemory(  width * height * bit_depth );
//    ICSharedMemory* sm = ShareMemManager->getObject( PixelBufferHandler );
//    rgba            = (_PUCHAR)sm->obtainMemoryLock();
//
//    row_pointers    = (png_bytep*)malloc(sizeof(png_bytep) * height);
//
//    for ( _INT y = 0; y < height; y++)
//    {
//        row_pointers[y] = (png_bytep)malloc(width<<2); //each pixel(RGBA) has 4 bytes
//    }
//
//    png_read_image(png_ptr, row_pointers);
//
//    //unlike store the pixel data from top-left corner, store them from bottom-left corner for OGLES Texture drawing...
//    int pos = (width * height * 4) - (4 * width);
//    for( _INT row = 0; row < height; row++)
//    {
//        for( _INT col = 0; col < (4 * width); col += 4)
//        {
//            rgba[pos++] = row_pointers[row][col];        // red
//            rgba[pos++] = row_pointers[row][col + 1];    // green
//            rgba[pos++] = row_pointers[row][col + 2];    // blue
//            rgba[pos++] = row_pointers[row][col + 3];    // alpha
//        }
//        pos=(pos - (width * 4)*2); //move the pointer back two rows
//    }
//
//    for ( _INT k=0; k<height; k++)
//        free(row_pointers[k]);
//
//    free(row_pointers);
//
//    //clean up after the read, and free any memory allocated
//    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
//
//    sm->releaseMemoryLock();
//    return _TRUE;
//}


//1.JPEG
//- �ļ�ͷ��ʶ (2 bytes): 0xff, 0xd8 (SOI) (JPEG �ļ���ʶ)
//- �ļ������ʶ (2 bytes): 0xff, 0xd9 (EOI)
//
//2.TGA
//- δѹ����ǰ5�ֽ�    00 00 02 00 00
//- RLEѹ����ǰ5�ֽ�   00 00 10 00 00
//
//3.PNG
//- �ļ�ͷ��ʶ (8 bytes)   89 50 4E 47 0D 0A 1A 0A
//
//4.GIF
//- �ļ�ͷ��ʶ (6 bytes)   47 49 46 38 39(37) 61
//G    I   F   8    9 (7)   a
//
//5.BMP
//- �ļ�ͷ��ʶ (2 bytes)   42 4D
//B  M
//
//6.PCX
//- �ļ�ͷ��ʶ (1 bytes)   0A
//
//7.TIFF
//- �ļ�ͷ��ʶ (2 bytes)   4D 4D �� 49 49
//
//8.ICO
//- �ļ�ͷ��ʶ (8 bytes)   00 00 01 00 01 00 20 20
//
//9.CUR
//- �ļ�ͷ��ʶ (8 bytes)   00 00 02 00 01 00 20 20
//
//10.IFF
//- �ļ�ͷ��ʶ (4 bytes)   46 4F 52 4D
//F   O  R  M
//
//11.ANI
//- �ļ�ͷ��ʶ (4 bytes)   52 49 46 46
//R   I    F   F

_UCHAR  JpegSign[2] = { 0xff, 0xd8 };
_UCHAR  PngSign[2] = { 0x89, 0x50 };
_UCHAR  BmpSign[2] = { 0x42, 0x4D };


_BOOL getImagePixelBuffer( _PVOID buffer, _ULONG bufferSize, _HANDLE& PixelBufferHandler, _UINT& width, _UINT& height )
{
   _USHORT* sign = (_PUSHORT)buffer;

   if ( *sign == *(_PUSHORT)JpegSign )
   {

#ifndef USE_LIB_JPEG
        return Call_Java_IMAGE_FUNCTION( buffer, bufferSize, PixelBufferHandler, width, height );
#else
        return JpegImage( buffer, bufferSize, PixelBufferHandler, width, height );
#endif
   }
   else
   {
        return Call_Java_IMAGE_FUNCTION( buffer, bufferSize, PixelBufferHandler, width, height );
   }
   return _FALSE;
}


//log
extern "C"
{
    _VOID native_log( _CHAR* message )
    {
#ifdef  DEBUG
        __android_log_print(ANDROID_LOG_ERROR, "BaiDuRenderEngine", "%s", message);
#endif
    }
}
//

