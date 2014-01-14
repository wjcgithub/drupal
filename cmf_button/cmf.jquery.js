(function ($){
  Drupal.behaviors.cmf_click = {
    attach:function (context,settings){
      jQuery(settings.data.cmf.clickid).click(function (){
        // alert(1); 
        var thisid = jQuery(this).attr("id");
        var strs= new Array();
        var btnid = '';
        strs = thisid.split('-');
        for (i=0;i<strs.length ;i++ ){   
          if(i==0){
            btnid +='-'+strs[i]+'-filter';
          }else{
            btnid +='-'+strs[i];
          }
        }  
        btnid = btnid.substr(1,btnid.length);
        jQuery('#'+btnid).attr('checked','checked');
      });
    }
  }
})(jQuery);
