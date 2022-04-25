


#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <cairo/cairo.h>
#include "resvg.h"



SEXP resvg_(SEXP data_, SEXP zoom_) {
  resvg_init_log();

  resvg_options *opt = resvg_options_create();
  resvg_options_load_system_fonts(opt);

  resvg_render_tree *tree;
  // int err = resvg_parse_tree_from_file(argv[1], opt, &tree);
  int err = resvg_parse_tree_from_data((const char *)RAW(data_), length(data_), opt, &tree);

  resvg_options_destroy(opt);
  if (err != RESVG_OK)
  {
    Rprintf("Error id: %i\n", err);
    return R_NilValue;
  }

  double  zoom = asReal(zoom_);

  // resvg_fit_to fit_to = { RESVG_FIT_TO_TYPE_ORIGINAL, 1 };
  resvg_fit_to fit_to = { RESVG_FIT_TO_TYPE_ZOOM, zoom };

  resvg_size size = resvg_get_image_size(tree);
  int width  = (int)size.width  * zoom;
  int height = (int)size.height * zoom;

  cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);

  /* resvg doesn't support stride, so cairo_surface_t should have no padding */
  assert(cairo_image_surface_get_stride(surface) == (int)size.width * 4);

  unsigned char *surface_data = cairo_image_surface_get_data(surface);


  resvg_render(tree, fit_to, resvg_transform_identity(), width, height, (char*)surface_data);


  // Allocate an integer matrix of class 'nativeRaster'
  SEXP image = PROTECT(Rf_allocVector(INTSXP, width*height));
  SEXP dim = PROTECT(Rf_allocVector(INTSXP, 2));
  INTEGER(dim)[0] = height;
  INTEGER(dim)[1] = width;
  Rf_setAttrib(image, R_DimSymbol, dim);
  SET_CLASS(image, Rf_mkString("nativeRaster"));

  // Get a pointer to the raw bytes from cairo in CAIRO_FORMAT_ARGB32
  unsigned char *nrp = (unsigned char *)INTEGER(image);
  memcpy(nrp, surface_data, width * height * 4);


  cairo_surface_destroy(surface);
  resvg_tree_destroy(tree);

  UNPROTECT(2);
  return image;
}
