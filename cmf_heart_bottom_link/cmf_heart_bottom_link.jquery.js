(function ($){
  Drupal.behaviors.cmf_heart_bottom_link = {
    attach:function (context,settings){
      jQuery(settings.data.cmf_heart_bottom_link.clickid).click(function (){
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

      
      //判断是否清空内容
      jQuery(settings.data.cmf_heart_bottom_link.clickid).keyup(function (){
        // alert(1); 
        var thiss = this;
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
        if(jQuery(thiss).val()==''){
          btnid = btnid.substr(1,btnid.length);
          jQuery('#'+btnid).attr('checked',false);
        }
      });
    }
  }
})(jQuery);
