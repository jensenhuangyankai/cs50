$(document).ready( function() {
  window.addEventListener('load', (event) => {
      $('.grid').masonry({
        itemSelector: '.grid-item',
        percentPosition: true,
        columnWidth: '.grid-item'
      });
  });

  window.lightGallery(
    document.getElementById("animated-thumbnails-gallery"),
    {
      autoplayFirstVideo: false,
      pager: false,
      galleryId: "nature",
      plugins: [lgZoom, lgThumbnail],
      mobileSettings: {
        controls: false,
        showCloseIcon: false,
        download: false,
        rotate: false
      }
    }
  );
});
