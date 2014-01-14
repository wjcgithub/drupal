(function ($){
  Drupal.behaviors.heart_search_form = {
    attach:function (context){
      jQuery('#edit-keys').click(function (){
        if (jQuery(this).val() =='请输入您想查找的内容') this.value='';
      });
      jQuery('#edit-keys').blur(function (){
        if (jQuery(this).val() =='') this.value='请输入您想查找的内容';
      });
    }
  };
})(jQuery);