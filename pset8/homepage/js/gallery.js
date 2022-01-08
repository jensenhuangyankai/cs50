$(document).ready( function() {
  window.addEventListener('load', (event) => {
      $('.grid').masonry({
        itemSelector: '.grid-item',
        percentPosition: true,
        columnWidth: '.grid-sizer'
      });
  });
});
