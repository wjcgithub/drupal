(function ($){
  Drupal.behaviors.heart_search = {
    attach:function (context){
      jQuery('#search-block-form input[type=submit]').attr('disabled',true);
      jQuery('#search-block-form input[type=submit]').addClass('disabled');

      jQuery('#edit-search-block-form--2').click(function (){
        if (jQuery(this).val() =='请输入关键字') this.value='';
      });
      jQuery('#edit-search-block-form--2').blur(function (){
        if (jQuery(this).val().trim() =='') this.value='请输入关键字';
      });
      jQuery('#edit-search-block-form--2').keyup(function (){
        if (jQuery(this).val() =='请输入关键字' || jQuery(this).val().trim() =='') {
          jQuery('#search-block-form input[type=submit]').attr('disabled',true);
          jQuery('#search-block-form input[type=submit]').addClass('disabled');
        } else {
          jQuery('#search-block-form input[type=submit]').attr('disabled',false);
          jQuery('#search-block-form input[type=submit]').removeClass('disabled');
        }
      });
    }
  };
})(jQuery);