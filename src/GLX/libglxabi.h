/*
 * Copyright (c) 2013, NVIDIA CORPORATION.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * unaltered in all copies or substantial portions of the Materials.
 * Any additions, deletions, or changes to the original source files
 * must be clearly indicated in accompanying documentation.
 *
 * If only executable code is distributed, then the accompanying
 * documentation must state that "this software is based in part on the
 * work of the Khronos Group."
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 */

#if !defined(__LIB_GLX_ABI_H)
#define __LIB_GLX_ABI_H

#include <stdint.h>
#include <GL/glx.h>

#include "GLdispatchABI.h"

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * \defgroup glxvendorabi GLX Vendor ABI
 *
 * Definition of ABI exported by libGLX.so to libGLX_VENDOR.so libraries.
 *
 * Each vendor is associated with three distinct dispatch table types:
 *
 * - static GLX dispatch table: this is the fixed list of GLX 1.4 entrypoints
 *   provided by the vendor at load time during the initial handshake.
 * - dynamic GLX dispatch table: this is a structure allocated by the API
 *   library at runtime used to manage GLX extension functions which are not
 *   present in the static table.
 * - core GL dispatch table: this is a structure maintained by the API library
 *   which contains both GL core (static) and GL extension (dynamic) functions.
 *
 * @{
 */

/*!
 * Current version of the ABI.
 */
#define GLX_VENDOR_ABI_VERSION 0


/*!
 * This opaque structure stores function pointers for GLX extension functions.
 * It is allocated at runtime by the API library. Vendor-provided dispatch
 * functions retrieve and operate on this structure using the API below.
 */
typedef struct __GLXdispatchTableDynamicRec __GLXdispatchTableDynamic;

/*!
 * Forward declaration for createGLDispatch export.
 */
typedef struct __GLXvendorCallbacksRec __GLXvendorCallbacks;

/****************************************************************************
 * API library exports                                                      *
 ****************************************************************************/

typedef struct __GLXapiExportsRec {
    /************************************************************************
     * The following routines are used by vendor-implemented GLX dispatch
     * functions to lookup and call into the right vendor.
     ************************************************************************/

    /*!
     * This fetches the appropriate dynamic GLX dispatch table given the display
     * and screen number.
     */
    __GLXdispatchTableDynamic *(*getDynDispatch)(Display *dpy,
                                                 const int screen);

    /*!
     * This function retrieves the appropriate current dynamic dispatch table,
     * if a GL context is current. Otherwise, this returns NULL.
     */
    __GLXdispatchTableDynamic *(*getCurrentDynDispatch)(void);

    /*!
     * This function retrieves an entry point from the dynamic dispatch table
     * given an index into the table.
     */
    __GLXextFuncPtr           (*fetchDispatchEntry)
        (__GLXdispatchTableDynamic *dynDispatch, int index);

    /************************************************************************
     * This routine is used by the vendor to lookup its context structure.
     * The contents of this structure are opaque to the API library and
     * vendor-dependent.
     ************************************************************************/

    /*!
     * This retrieves the current context for this thread.
     */
    GLXContext                (*getCurrentContext)(void);

    /************************************************************************
     * These routines are used by vendor dispatch functions to look up
     * and add mappings between various objects and screens.
     ************************************************************************/

    void (*addScreenContextMapping)(GLXContext context, int screen);
    void (*removeScreenContextMapping)(GLXContext context);
    int  (*screenFromContext)(GLXContext context);

    void (*addScreenFBConfigMapping)(GLXFBConfig config, int screen);
    void (*removeScreenFBConfigMapping)(GLXFBConfig config);
    int  (*screenFromFBConfig)(GLXFBConfig config);

    void (*addScreenDrawableMapping)(GLXDrawable drawable, int screen);
    void (*removeScreenDrawableMapping)(GLXDrawable drawable);
    int  (*screenFromDrawable)(Display *dpy, GLXDrawable drawable);

} __GLXapiExports;

/*****************************************************************************
 * API library imports                                                       *
 *****************************************************************************/

/*!
 * This structure stores function pointers for all functions defined in GLX 1.4.
 */
typedef struct __GLX14EntryPointsRec {
    XVisualInfo* (*chooseVisual)          (Display *dpy,
                                           int screen,
                                           int *attrib_list);

    void         (*copyContext)           (Display *dpy,
                                           GLXContext src,
                                           GLXContext dst,
                                           unsigned long mask);

    GLXContext   (*createContext)         (Display *dpy,
                                           XVisualInfo *vis,
                                           GLXContext share_list,
                                           Bool direct);

    GLXPixmap    (*createGLXPixmap)       (Display *dpy,
                                           XVisualInfo *vis,
                                           Pixmap pixmap);

    void         (*destroyContext)        (Display *dpy,
                                           GLXContext ctx);

    void         (*destroyGLXPixmap)      (Display *dpy,
                                           GLXPixmap pix);

    int          (*getConfig)             (Display *dpy,
                                           XVisualInfo *vis,
                                           int attrib,
                                           int *value);

    Bool         (*isDirect)              (Display *dpy,
                                           GLXContext ctx);

    Bool         (*makeCurrent)           (Display *dpy,
                                           GLXDrawable drawable,
                                           GLXContext ctx);

    void         (*swapBuffers)           (Display *dpy,
                                           GLXDrawable drawable);

    void         (*useXFont)              (Font font,
                                           int first,
                                           int count,
                                           int list_base);

    void         (*waitGL)                (void);

    void         (*waitX)                 (void);

    const char*  (*queryServerString)     (Display *dpy,
                                           int screen,
                                           int name);

    const char*  (*getClientString)     (Display *dpy,
                                         int name);

    const char*  (*queryExtensionsString) (Display *dpy,
                                           int screen);

    GLXFBConfig* (*chooseFBConfig)        (Display *dpy,
                                           int screen,
                                           const int *attrib_list,
                                           int *nelements);

    GLXContext   (*createNewContext)      (Display *dpy,
                                           GLXFBConfig config,
                                           int render_type,
                                           GLXContext share_list,
                                           Bool direct);

    GLXPbuffer   (*createPbuffer)         (Display *dpy,
                                           GLXFBConfig config,
                                           const int *attrib_list);

    GLXPixmap    (*createPixmap)          (Display *dpy,
                                           GLXFBConfig config,
                                           Pixmap pixmap,
                                           const int *attrib_list);

    GLXWindow    (*createWindow)          (Display *dpy,
                                           GLXFBConfig config,
                                           Window win,
                                           const int *attrib_list);

    void         (*destroyPbuffer)        (Display *dpy,
                                           GLXPbuffer pbuf);

    void         (*destroyPixmap)         (Display *dpy,
                                           GLXPixmap pixmap);

    void         (*destroyWindow)         (Display *dpy,
                                           GLXWindow win);

    int          (*getFBConfigAttrib)     (Display *dpy,
                                           GLXFBConfig config,
                                           int attribute,
                                           int *value);

    GLXFBConfig* (*getFBConfigs)          (Display *dpy,
                                           int screen,
                                           int *nelements);

    void         (*getSelectedEvent)      (Display *dpy,
                                           GLXDrawable draw,
                                           unsigned long *event_mask);

    XVisualInfo* (*getVisualFromFBConfig) (Display *dpy,
                                           GLXFBConfig config);

    Bool         (*makeContextCurrent)    (Display *dpy, GLXDrawable draw,
                                           GLXDrawable read, GLXContext ctx);

    int          (*queryContext)          (Display *dpy,
                                           GLXContext ctx,
                                           int attribute,
                                           int *value);

    void         (*queryDrawable)         (Display *dpy,
                                           GLXDrawable draw,
                                           int attribute,
                                           unsigned int *value);

    void         (*selectEvent)           (Display *dpy,
                                           GLXDrawable draw,
                                           unsigned long event_mask);
} __GLX14EntryPoints;

/*!
 * This structure stores required and optional vendor library callbacks.
 */
struct __GLXvendorCallbacksRec {
    /*!
     * This retrieves the pointer to the real GLX or core GL function.
     * isClientAPI indicates whether libglvnd thinks this function is
     * from GLX or a client API.  This can be used in vendor libraries
     * for internal consistency checks.
     */
    void        *(*getProcAddress)        (const GLubyte *procName,
                                           int isClientAPI);

    /*!
     * This retrieves vendor-neutral functions which use the
     * __GLXdispatchTableDynamic API above to dispatch to the correct vendor.
     */
    void        *(*getDispatchAddress)    (const GLubyte *procName);

    /*!
     * This notifies the vendor library which dispatch table index is
     * assigned to a particular GLX extension function.
     */
    void        (*setDispatchIndex)      (const GLubyte *procName, int index);

    /*!
     * (OPTIONAL) Callbacks by which the vendor library may re-write libglvnd's
     * entrypoints at make current time, provided no other contexts are current
     * and the TLS model supports this functionality.  This is a performance
     * optimization that may not be available at runtime; the vendor library
     * must not depend on this functionality for correctness.  This should
     * point to a statically-allocated structure, or NULL if unimplemented.
     */
    const __GLdispatchPatchCallbacks *patchCallbacks;

};

typedef struct __GLXapiImportsRec {
    __GLX14EntryPoints glx14ep;
    __GLXvendorCallbacks glxvc;
} __GLXapiImports;

/*****************************************************************************/

/*!
 * Vendor libraries must export a function called __glx_Main() with the
 * following prototype. This function also performs a handshake based on the ABI
 * version number. This function receives a pointer to an exports table whose
 * lifetime is only guaranteed to be at a minimum that of the call to
 * __glx_Main(), in addition to the version number and a string identifying the
 * vendor. If there is an ABI version mismatch or some other error occurs, this
 * function returns NULL; otherwise this returns a pointer to a filled-in
 * dispatch table.
 */
#define __GLX_MAIN_PROTO_NAME "__glx_Main"
#define __GLX_MAIN_PROTO(version, exports, vendorName)                \
    const __GLXapiImports *__glx_Main(uint32_t version,               \
                                      const __GLXapiExports *exports, \
                                      const char *vendorName,         \
                                      int vendorID)

typedef const __GLXapiImports *(*__PFNGLXMAINPROC)
    (uint32_t, const __GLXapiExports *, const char *, int);

/*!
 * @}
 */

#if defined(__cplusplus)
}
#endif

#endif /* __LIB_GLX_ABI_H */
