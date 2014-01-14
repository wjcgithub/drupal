(function($){ 

  $(document).ready(function(){

    // initialise plugin
    var example = $('#jquery_menu').superfish({
      //add options here if required
      // speed:'fast',
      delay:50,
  });

  $('#superfish #jquery_menu_superfish').mouseover(function (){
      $('#nav').css('z-index','-100');
    });
  $('#jquery_menu_superfish').mouseout(function (){
      $('#nav').css('z-index','200');
    });
  });

})(jQuery);