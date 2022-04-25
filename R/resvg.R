


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Render an SVG to a nativeRaster object
#'
#' @param svg_text svg as single text string
#' @param zoom zoom factor. Default NULL means to use the natural SVG size.
#'
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
resvg <- function(svg_text, zoom = 1) {
  .Call(resvg_, charToRaw(svg_text), zoom)
}



if (FALSE) {
  svg_text <- '
  <svg viewBox="0 0 100 100" xmlns="http://www.w3.org/2000/svg">
    <rect width="100" height="100" fill="#555555"
        style="stroke: black; stroke-width: 20"/>
    <circle class="pale" cx="50" cy="50" r="20" fill="white" />
  </svg>
  '

  xx <- resvg(svg_text)
  library(grid)
  grid.draw( grid.raster(xx))

  yy <- rsvg::rsvg_nativeraster(charToRaw(svg_text))
  grid.newpage()
  grid.draw( grid.raster(yy))


  library(rsvg)

  bench::mark(
    resvg(svg_text),
    rsvg_nativeraster(charToRaw(svg_text)),
    check = FALSE
  )

}
