/* $XFree86$ */ /* -*- mode: c; c-basic-offset: 3 -*- */
/*
 * Copyright 2000 Gareth Hughes
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * GARETH HUGHES BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Authors:
 *	Gareth Hughes <gareth@valinux.com>
 *	Leif Delgass <ldelgass@retinalburn.net>
 *	Jos� Fonseca <j_r_fonseca@yahoo.co.uk>
 */

#include "glheader.h"
#include "imports.h"
#include "context.h"
#include "macros.h"
#include "texformat.h"

#include "mach64_context.h"
#include "mach64_ioctl.h"
#include "mach64_state.h"
#include "mach64_vb.h"
#include "mach64_tris.h"
#include "mach64_tex.h"

static void mach64SetTexImages( mach64ContextPtr mmesa,
                              const struct gl_texture_object *tObj )
{
   mach64TexObjPtr t = (mach64TexObjPtr) tObj->DriverData;
   struct gl_texture_image *baseImage = tObj->Image[0][tObj->BaseLevel];
#if 0
   int log2Pitch, log2Height, log2Size, log2MinSize;
   int i;
   GLint firstLevel, lastLevel;
#endif
   int totalSize;

   assert(t);
   assert(baseImage);

   if ( MACH64_DEBUG & DEBUG_VERBOSE_API )
      fprintf( stderr, "%s( %p )\n", __FUNCTION__, tObj );

   switch (baseImage->TexFormat->MesaFormat) {
   case MESA_FORMAT_ARGB8888:
      t->textureFormat = MACH64_DATATYPE_ARGB8888;
      break;
   case MESA_FORMAT_ARGB4444:
      t->textureFormat = MACH64_DATATYPE_ARGB4444;
      break;
   case MESA_FORMAT_RGB565:
      t->textureFormat = MACH64_DATATYPE_RGB565;
      break;
   case MESA_FORMAT_ARGB1555:
      t->textureFormat = MACH64_DATATYPE_ARGB1555;
      break;
   case MESA_FORMAT_RGB332:
      t->textureFormat = MACH64_DATATYPE_RGB332;
      break;
   case MESA_FORMAT_RGB888:
      t->textureFormat = MACH64_DATATYPE_RGB8;
      break;
   case MESA_FORMAT_CI8:
      t->textureFormat = MACH64_DATATYPE_CI8;
      break;
   case MESA_FORMAT_YCBCR:
      t->textureFormat = MACH64_DATATYPE_YVYU422;
      break;
   case MESA_FORMAT_YCBCR_REV:
      t->textureFormat = MACH64_DATATYPE_VYUY422;
      break;
   default:
      _mesa_problem(mmesa->glCtx, "Bad texture format in %s", __FUNCTION__);
   };

#if 0
   /* Compute which mipmap levels we really want to send to the hardware.
    * This depends on the base image size, GL_TEXTURE_MIN_LOD,
    * GL_TEXTURE_MAX_LOD, GL_TEXTURE_BASE_LEVEL, and GL_TEXTURE_MAX_LEVEL.
    * Yes, this looks overly complicated, but it's all needed.
    */
   firstLevel = tObj->BaseLevel + (GLint) (tObj->MinLod + 0.5);
   firstLevel = MAX2(firstLevel, tObj->BaseLevel);
   lastLevel = tObj->BaseLevel + (GLint) (tObj->MaxLod + 0.5);
   lastLevel = MAX2(lastLevel, tObj->BaseLevel);
   lastLevel = MIN2(lastLevel, tObj->BaseLevel + baseImage->MaxLog2);
   lastLevel = MIN2(lastLevel, tObj->MaxLevel);
   lastLevel = MAX2(firstLevel, lastLevel); /* need at least one level */

   log2Pitch = tObj->Image[firstLevel]->WidthLog2;
   log2Height = tObj->Image[firstLevel]->HeightLog2;
   log2Size = MAX2(log2Pitch, log2Height);
   log2MinSize = log2Size;

   t->dirty = 0;
   totalSize = 0;
   for ( i = firstLevel; i <= lastLevel; i++ ) {
      const struct gl_texture_image *texImage;

      texImage = tObj->Image[i];
      if ( !texImage || !texImage->Data ) {
         lastLevel = i - 1;
	 break;
      }

      log2MinSize = texImage->MaxLog2;

      t->image[i - firstLevel].offset = totalSize;
      t->image[i - firstLevel].width  = tObj->Image[i]->Width;
      t->image[i - firstLevel].height = tObj->Image[i]->Height;

      t->dirty |= (1 << i);

      totalSize += (tObj->Image[i]->Height *
		    tObj->Image[i]->Width *
		    tObj->Image[i]->TexFormat->TexelBytes);

      /* Offsets must be 32-byte aligned for host data blits and tiling */
      totalSize = (totalSize + 31) & ~31;
   }

   t->totalSize = totalSize;
   t->firstLevel = firstLevel;
   t->lastLevel = lastLevel;

   /* Set the texture format */
   t->setup.tex_cntl &= ~(0xf << 16);
   t->setup.tex_cntl |= t->textureFormat;

   t->setup.tex_combine_cntl = 0x00000000;  /* XXX is this right? */

   t->setup.tex_size_pitch = ((log2Pitch   << R128_TEX_PITCH_SHIFT) |
			      (log2Size    << R128_TEX_SIZE_SHIFT) |
			      (log2Height  << R128_TEX_HEIGHT_SHIFT) |
			      (log2MinSize << R128_TEX_MIN_SIZE_SHIFT));

   for ( i = 0 ; i < R128_MAX_TEXTURE_LEVELS ; i++ ) {
      t->setup.tex_offset[i]  = 0x00000000;
   }

   if (firstLevel == lastLevel)
      t->setup.tex_cntl |= R128_MIP_MAP_DISABLE;
   else
      t->setup.tex_cntl &= ~R128_MIP_MAP_DISABLE;

#else
   if ( ( baseImage->Format == GL_RGBA ) ||
	( baseImage->Format == GL_ALPHA ) ||
	( baseImage->Format == GL_LUMINANCE_ALPHA ) ) {
      t->hasAlpha = 1;
   } else {
      t->hasAlpha = 0;
   }

   totalSize = ( baseImage->Width * baseImage->Height * 
      baseImage->TexFormat->TexelBytes );
   totalSize = (totalSize + 31) & ~31;
   t->size = totalSize;
   t->widthLog2 = baseImage->WidthLog2;
   t->heightLog2 = baseImage->HeightLog2;
   t->maxLog2 = baseImage->MaxLog2;
   
#endif
}

static void mach64UpdateTextureEnv( GLcontext *ctx, int unit )
{
   mach64ContextPtr mmesa = MACH64_CONTEXT(ctx);
   GLint source = mmesa->tmu_source[unit];
   const struct gl_texture_unit *texUnit = &ctx->Texture.Unit[source];
   const struct gl_texture_object *tObj = texUnit->_Current;
   const GLenum format = tObj->Image[0][tObj->BaseLevel]->Format;
   GLuint s = mmesa->setup.scale_3d_cntl;

   if ( MACH64_DEBUG & DEBUG_VERBOSE_API ) {
      fprintf( stderr, "%s( %p, %d )\n",
	       __FUNCTION__, ctx, unit );
   }

/*                 REPLACE  MODULATE   DECAL              GL_BLEND
 *
 * ALPHA           C = Cf   C = Cf     undef              C = Cf
 *                 A = At   A = AfAt                      A = AfAt
 *
 * LUMINANCE       C = Ct   C = CfCt   undef              C = Cf(1-Ct)+CcCt 
 *                 A = Af   A = Af                        A = Af
 *
 * LUMINANCE_ALPHA C = Ct   C = CfCt   undef              C = Cf(1-Ct)+CcCt
 *                 A = At   A = AfAt                      A = AfAt
 *
 * INTENSITY       C = Ct   C = CfCt   undef              C = Cf(1-Ct)+CcCt
 *                 A = At   A = AfAt                      A = Af(1-At)+AcAt
 *
 * RGB             C = Ct   C = CfCt   C = Ct             C = Cf(1-Ct)+CcCt
 *                 A = Af   A = Af     A = Af             A = Af
 *
 * RGBA            C = Ct   C = CfCt   C = Cf(1-At)+CtAt  C = Cf(1-Ct)+CcCt
 *                 A = At   A = AfAt   A = Af             A = AfAt 
 */


   if ( unit == 0 ) {
      s &= ~MACH64_TEX_LIGHT_FCN_MASK;

      /* Set the texture environment state 
       * Need to verify these are working correctly, but the
       * texenv Mesa demo seems to work.
       */
      switch ( texUnit->EnvMode ) {
      case GL_REPLACE:
	 switch ( format ) {
	 case GL_ALPHA:
	 case GL_LUMINANCE_ALPHA:
	 case GL_INTENSITY:
	    /* Not compliant - can't get At */
	    FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_TRUE );
	    s |= MACH64_TEX_LIGHT_FCN_MODULATE;
	    break;
	 default:
	    s |= MACH64_TEX_LIGHT_FCN_REPLACE;
	 }
	 break;
      case GL_MODULATE:
	 switch ( format ) {
	 case GL_ALPHA:
	    FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_TRUE );
	    s |= MACH64_TEX_LIGHT_FCN_MODULATE;
	    break;
	 case GL_RGB:
	 case GL_LUMINANCE:
	    /* These should be compliant */
	    s |= MACH64_TEX_LIGHT_FCN_MODULATE;
	    break;
	 case GL_LUMINANCE_ALPHA:
	 case GL_INTENSITY:
	    FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_TRUE );
	    s |= MACH64_TEX_LIGHT_FCN_MODULATE;
	    break;
	 case GL_RGBA:
	    /* Should fallback when blending enabled for complete compliance */
	    s |= MACH64_TEX_LIGHT_FCN_MODULATE;
	    break;
	 default:
	    s |= MACH64_TEX_LIGHT_FCN_MODULATE;
	 }
	 break;
      case GL_DECAL:
	 switch ( format ) {
	 case GL_RGBA: 
	    s |= MACH64_TEX_LIGHT_FCN_ALPHA_DECAL;
	    break;
	 case GL_RGB:
	    s |= MACH64_TEX_LIGHT_FCN_REPLACE;
	    break;
	 case GL_ALPHA:
	 case GL_LUMINANCE_ALPHA:
	    /* undefined - disable texturing, pass fragment unmodified  */
	    /* Also, pass fragment alpha instead of texture alpha */
	    s &= ~MACH64_TEX_MAP_AEN;
	    s |= MACH64_TEXTURE_DISABLE;
	    s |= MACH64_TEX_LIGHT_FCN_MODULATE;
	    break;
	 case GL_LUMINANCE:
	 case GL_INTENSITY:
	    /* undefined - disable texturing, pass fragment unmodified  */
	    s |= MACH64_TEXTURE_DISABLE;
	    s |= MACH64_TEX_LIGHT_FCN_MODULATE;
	    break;
	 default:
	    s |= MACH64_TEX_LIGHT_FCN_MODULATE;
	 }
	 break;
      case GL_BLEND:
	 /* GL_BLEND not supported by RagePRO, use software */
	 FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_TRUE );
	 s |= MACH64_TEX_LIGHT_FCN_MODULATE;
	 break;
      case GL_ADD:
      case GL_COMBINE:
	 FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_TRUE );
	 s |= MACH64_TEX_LIGHT_FCN_MODULATE;
	 break;
      default:
	 s |= MACH64_TEX_LIGHT_FCN_MODULATE;
      }

      if ( mmesa->setup.scale_3d_cntl != s ) {
	 mmesa->setup.scale_3d_cntl = s;
	 mmesa->dirty |= MACH64_UPLOAD_SCALE_3D_CNTL;
      }

   } else {
      /* blend = 0, modulate = 1 - initialize to blend */
      mmesa->setup.tex_cntl &= ~MACH64_COMP_COMBINE_MODULATE;
      /* Set the texture composite function for multitexturing*/
      switch ( texUnit->EnvMode ) {
      case GL_BLEND:
	 /* GL_BLEND not supported by RagePRO, use software */
	 FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_TRUE );
	 mmesa->setup.tex_cntl |= MACH64_COMP_COMBINE_MODULATE;
	 break;
      case GL_MODULATE:
	 /* Should fallback when blending enabled for complete compliance */
	 mmesa->setup.tex_cntl |= MACH64_COMP_COMBINE_MODULATE;
	 break;
      case GL_REPLACE:
	 switch ( format ) {
	 case GL_ALPHA:
	    mmesa->setup.tex_cntl |= MACH64_COMP_COMBINE_MODULATE;
	    break;
	 default: /* not supported by RagePRO */
	    FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_TRUE );
	    mmesa->setup.tex_cntl |= MACH64_COMP_COMBINE_MODULATE;
	 }
	 break;
      case GL_DECAL:
	 switch ( format ) {
	 case GL_ALPHA:
	 case GL_LUMINANCE:
	 case GL_LUMINANCE_ALPHA:
	 case GL_INTENSITY:
	    /* undefined, disable compositing and pass fragment unmodified */
	    mmesa->setup.tex_cntl &= ~MACH64_TEXTURE_COMPOSITE;
	    break;
	 default: /* not supported by RagePRO */
	    FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_TRUE );
	    mmesa->setup.tex_cntl |= MACH64_COMP_COMBINE_MODULATE;
	 }
	 break;
      case GL_ADD:
      case GL_COMBINE:
	 FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_TRUE );
	 mmesa->setup.tex_cntl |= MACH64_COMP_COMBINE_MODULATE;
	 break;
      default:
	 mmesa->setup.tex_cntl |= MACH64_COMP_COMBINE_MODULATE;
      }
   }
}


static void mach64UpdateTextureUnit( GLcontext *ctx, int unit )
{
   mach64ContextPtr mmesa = MACH64_CONTEXT(ctx);
   int source = mmesa->tmu_source[unit];
   const struct gl_texture_unit *texUnit = &ctx->Texture.Unit[source];
   const struct gl_texture_object *tObj = ctx->Texture.Unit[source]._Current;
   mach64TexObjPtr t = tObj->DriverData;
   GLuint d = mmesa->setup.dp_pix_width;
   GLuint s = mmesa->setup.scale_3d_cntl;

   assert(unit == 0 || unit == 1);  /* only two tex units */

   if ( MACH64_DEBUG & DEBUG_VERBOSE_API ) {
      fprintf( stderr, "%s( %p, %d ) enabled=0x%x 0x%x\n",
	       __FUNCTION__, ctx, unit, ctx->Texture.Unit[0]._ReallyEnabled,
	       ctx->Texture.Unit[1]._ReallyEnabled);
   }

   if (texUnit->_ReallyEnabled & (TEXTURE_1D_BIT | TEXTURE_2D_BIT)) {

      assert(t);  /* should have driver tex data by now */

      /* Fallback if there's a texture border */
      if ( tObj->Image[0][tObj->BaseLevel]->Border > 0 ) {
         FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_TRUE );
         return;
      }

      /* Upload teximages */
      if (t->dirty) {
         mach64SetTexImages( mmesa, tObj );
	 mmesa->dirty |= (MACH64_UPLOAD_TEX0IMAGE << unit);
      }

      /* Bind to the given texture unit */
      mmesa->CurrentTexObj[unit] = t;
      t->bound |= (1 << unit);

      if ( t->memBlock )
         mach64UpdateTexLRU( mmesa, t );

      /* register setup */
      if ( unit == 0 ) {
         d &= ~MACH64_SCALE_PIX_WIDTH_MASK;
         d |= (t->textureFormat << 28);
   
         s &= ~(MACH64_TEXTURE_DISABLE |
		MACH64_TEX_CACHE_SPLIT |
		MACH64_TEX_BLEND_FCN_MASK |
		MACH64_TEX_MAP_AEN);
   
         if ( mmesa->multitex ) {
	    s |= MACH64_TEX_BLEND_FCN_TRILINEAR | MACH64_TEX_CACHE_SPLIT;
         } else if ( t->BilinearMin ) {
	    s |= MACH64_TEX_BLEND_FCN_LINEAR;
         } else {
	    s |= MACH64_TEX_BLEND_FCN_NEAREST;
         }
         if ( t->BilinearMag ) {
	    s |=  MACH64_BILINEAR_TEX_EN;
         } else {
	    s &= ~MACH64_BILINEAR_TEX_EN;
         }
   
         if ( t->hasAlpha ) {
	    s |= MACH64_TEX_MAP_AEN;
         }
   
         mmesa->setup.tex_cntl &= ~(MACH64_TEXTURE_CLAMP_S |
				    MACH64_TEXTURE_CLAMP_T |
				    MACH64_SECONDARY_STW);
   
         if ( t->ClampS ) {
	    mmesa->setup.tex_cntl |= MACH64_TEXTURE_CLAMP_S;
         }
         if ( t->ClampT ) {
	    mmesa->setup.tex_cntl |= MACH64_TEXTURE_CLAMP_T;
         }
   
         mmesa->setup.tex_size_pitch |= ((t->widthLog2  << 0) |
					 (t->maxLog2    << 4) |
					 (t->heightLog2 << 8));
      } else {
         
         /* Enable texture mapping mode */
         s &= ~MACH64_TEXTURE_DISABLE;
   
         d &= ~MACH64_COMPOSITE_PIX_WIDTH_MASK;
         d |= (t->textureFormat << 4);
   
         mmesa->setup.tex_cntl &= ~(MACH64_COMP_ALPHA |
				    MACH64_SEC_TEX_CLAMP_S |
				    MACH64_SEC_TEX_CLAMP_T);
         mmesa->setup.tex_cntl |= (MACH64_TEXTURE_COMPOSITE |
				   MACH64_SECONDARY_STW);
   
         if ( t->BilinearMin ) {
	    mmesa->setup.tex_cntl |= MACH64_COMP_BLEND_BILINEAR;
         } else {
	    mmesa->setup.tex_cntl &= ~MACH64_COMP_BLEND_BILINEAR;
         }
         if ( t->BilinearMag ) {
	    mmesa->setup.tex_cntl |=  MACH64_COMP_FILTER_BILINEAR;
         } else {
	    mmesa->setup.tex_cntl &= ~MACH64_COMP_FILTER_BILINEAR;
         }
         
         if ( t->hasAlpha ) {
	    mmesa->setup.tex_cntl |= MACH64_COMP_ALPHA;
         }
         if ( t->ClampS ) {
	    mmesa->setup.tex_cntl |= MACH64_SEC_TEX_CLAMP_S;
         }
         if ( t->ClampT ) {
	    mmesa->setup.tex_cntl |= MACH64_SEC_TEX_CLAMP_T;
         }
   
         mmesa->setup.tex_size_pitch |= ((t->widthLog2  << 16) |
					 (t->maxLog2    << 20) |
					 (t->heightLog2 << 24));
      }
   
      if ( mmesa->setup.scale_3d_cntl != s ) {
         mmesa->setup.scale_3d_cntl = s;
         mmesa->dirty |= MACH64_UPLOAD_SCALE_3D_CNTL;
      }
   
      if ( mmesa->setup.dp_pix_width != d ) {
         mmesa->setup.dp_pix_width = d;
         mmesa->dirty |= MACH64_UPLOAD_DP_PIX_WIDTH;
      }  
   }
   else if (texUnit->_ReallyEnabled) {
      /* 3D or cube map texture enabled - fallback */
      FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_TRUE );
   }
   else {
      /* texture unit disabled */
   }
}


/* Update the hardware texture state */
void mach64UpdateTextureState( GLcontext *ctx )
{
   mach64ContextPtr mmesa = MACH64_CONTEXT(ctx);

   if ( MACH64_DEBUG & DEBUG_VERBOSE_API ) {
      fprintf( stderr, "%s( %p ) en=0x%x 0x%x\n",
	       __FUNCTION__, ctx, ctx->Texture.Unit[0]._ReallyEnabled,
	       ctx->Texture.Unit[1]._ReallyEnabled);
   }

   /* Clear any texturing fallbacks */
   FALLBACK( mmesa, MACH64_FALLBACK_TEXTURE, GL_FALSE );

   /* Unbind any currently bound textures */
   if ( mmesa->CurrentTexObj[0] ) mmesa->CurrentTexObj[0]->bound = 0;
   if ( mmesa->CurrentTexObj[1] ) mmesa->CurrentTexObj[1]->bound = 0;
   mmesa->CurrentTexObj[0] = NULL;
   mmesa->CurrentTexObj[1] = NULL;

   /* Disable all texturing until it is known to be good */
   mmesa->setup.scale_3d_cntl  |=  MACH64_TEXTURE_DISABLE;
   mmesa->setup.scale_3d_cntl  &= ~MACH64_TEX_MAP_AEN;
   mmesa->setup.tex_cntl       &= ~MACH64_TEXTURE_COMPOSITE;

   mmesa->setup.tex_size_pitch = 0x00000000;

   mmesa->tmu_source[0] = 0;
   mmesa->tmu_source[1] = 1;
   mmesa->multitex = 0;

   if (ctx->Texture._EnabledUnits & 0x2) {
       /* unit 1 enabled */
       if (ctx->Texture._EnabledUnits & 0x1) {
	  /* units 0 and 1 enabled */
	  mmesa->multitex = 1;
	  mach64UpdateTextureUnit( ctx, 0 );
	  mach64UpdateTextureEnv( ctx, 0 );
	  mach64UpdateTextureUnit( ctx, 1 );
	  mach64UpdateTextureEnv( ctx, 1 );
       } else {
	  mmesa->tmu_source[0] = 1;
	  mmesa->tmu_source[1] = 0;
	  mach64UpdateTextureUnit( ctx, 0 );
	  mach64UpdateTextureEnv( ctx, 0 );
       }
   } else if (ctx->Texture._EnabledUnits & 0x1) {
      /* only unit 0 enabled */ 
      mach64UpdateTextureUnit( ctx, 0 );
      mach64UpdateTextureEnv( ctx, 0 );
   }

   mmesa->dirty |= (MACH64_UPLOAD_SCALE_3D_CNTL |
		    MACH64_UPLOAD_TEXTURE);
}

