window.MathJax = {
  tex: {
    inlineMath: [["\\(", "\\)"]],
    displayMath: [["\\[", "\\]"]],
    processEscapes: true,
    processEnvironments: true
  },
  options: {
    ignoreHtmlClass: ".*|",
    processHtmlClass: "arithmatex"
  }
};

document$.subscribe(() => {
  MathJax.startup.output.clearCache()
  MathJax.typesetClear()
  MathJax.texReset()
  MathJax.typesetPromise()
})

// Initialize VegaLite charts
document.addEventListener('DOMContentLoaded', function() {
  // Find all vegalite code blocks and render them
  document.querySelectorAll('.vegalite').forEach(function(element) {
    try {
      const spec = JSON.parse(element.textContent);
      vegaEmbed(element, spec, {
        theme: 'default',
        renderer: 'svg',
        actions: false
      });
    } catch (e) {
      console.error('Error rendering Vega-Lite chart:', e);
    }
  });
});