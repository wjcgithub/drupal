jQuery(function() {
    //jQuery('#node-admin-filter-result #edit-filters').append(jQuery('#main-filters').html());
    //jQuery('#main-filters').remove();
	
	jQuery('#headerafter').replaceWith(jQuery('#node-admin-filter-result'));
	jQuery('.form-item-created-before').parent().before('<div class="clearfix"></div>');
	jQuery('#edit-language').parent().before('<div class="clearfix"></div>');
	
    
    //alert('cmf.jquery.js file active');
   jQuery('#taxdiv #edit-category').live('change', function (){
   		var thiss = this;
   		var vid = jQuery(thiss).val();
   		var basepath = jQuery('#basepath').val(); 
   		//判断当前是否有二级栏目
	    delAllNextSelect(thiss);
   		addSearchIcon(thiss);
   		jQuery.ajax({
	        type:'post',
	        dataType:'json',
	        url:basepath+'cmf/getchildren',
	        data:'vid='+vid+'&parentid=0',
	        error:function(){
	        	//删除图标
	          	delSearchIcon(thiss);
	          	//判断当前是否有二级栏目
	    		delAllNextSelect(thiss);
	    		//为隐藏值赋值
	    		setHiddenVal();
	         	alert('对不起，意外错误请刷新重试！');
	        },
	        success:function(json){
	          if (json.ret == 1) { 
	          	//删除图标
	          	delSearchIcon(thiss);
	          	//判断当前是否有二级栏目
	    		delAllNextSelect(thiss);
	            //添加select
	            jQuery('#taxdiv .form-type-select').append(json.html);
	            //为隐藏值赋值
	    		setHiddenVal();
	          }else if(json.ret == 0){
	          	//删除图标
	          	delSearchIcon(thiss);
	          	//判断当前是否有二级栏目
	    		delAllNextSelect(thiss);
	    		//为隐藏值赋值
	    		setHiddenVal();
	          }
	          else {
	          	//删除图标
	          	delSearchIcon(thiss);
	          	//判断当前是否有二级栏目
	    		delAllNextSelect(thiss);
	    		//为隐藏值赋值
	    		setHiddenVal();
	           	alert('对不起，意外错误请刷新重试！');
	          }
	        }
      	});
   });

    jQuery('#taxdiv .taxchildren').live('change', function (){
   		var thiss = this;
   		var vid = jQuery('#taxdiv #edit-category').val();
   		var parentid = jQuery(thiss).val();
   		var basepath = jQuery('#basepath').val(); 
   		//判断当前是否有二级栏目
	    delAllNextSelect(thiss);
   		addSearchIcon(thiss);
   		jQuery.ajax({
	        type:'post',
	        dataType:'json',
	        url:basepath+'cmf/getchildren',
	        data:'vid='+vid+'&parentid='+parentid,
	        error:function(){
	        	//删除图标
	          	delSearchIcon(thiss);
	          	//判断当前是否有二级栏目
	    		delAllNextSelect(thiss);
	    		//为隐藏值赋值
	    		setHiddenVal();
	         	alert('对不起，意外错误请刷新重试！');
	        },
	        success:function(json){
	          if (json.ret == 1) { 
	          	//删除图标
	          	delSearchIcon(thiss);
	          	//判断当前是否有二级栏目
	    		delAllNextSelect(thiss);
	    		//添加select
	            jQuery('#taxdiv .form-type-select').append(json.html);
	            //为隐藏值赋值
	    		setHiddenVal();
	          }else if(json.ret == 0){
	          	//删除图标
	          	delSearchIcon(thiss);
	          	//判断当前是否有二级栏目
	    		delAllNextSelect(thiss);
	    		//为隐藏值赋值
	    		setHiddenVal();
	          }
	          else {
	          	//删除图标
	          	delSearchIcon(thiss);
	          	//判断当前是否有二级栏目
	    		delAllNextSelect(thiss);
	    		//为隐藏值赋值
	    		setHiddenVal();
	           	alert('对不起，意外错误请刷新重试！');
	          }
	        }
      	});
   });
    
  });

/**
 * 添加搜索图标
 */
function addSearchIcon(thiss){ 
	if(!jQuery(thiss).parent().find('div.on-search').length){ 
		jQuery(thiss).parent().append('<div class="on-search"></div>');
	}
}

/**
* 删除搜索图标
*/
function delSearchIcon(thiss){
	jQuery(thiss).parent().find('.on-search').remove();
}

/**
* 删除所有的select
*/
function delAllNextSelect(thiss){
	if(jQuery(thiss).nextAll('select').length>0){ 
		jQuery(thiss).nextAll('select').remove();
		jQuery(thiss).nextAll('.empty').remove();
	}

	
}

/**
* 为隐藏tax赋值
*/
function setHiddenVal(){
	//为隐藏tax赋值
	var sellength = jQuery('#taxdiv select').length;
	var hideval;
	if(sellength>1){
		hideobj = jQuery('#taxdiv select:last');
		hideval = jQuery(hideobj).val();
		//如果没有选择的继续向上找
		if(hideval<0 && sellength>2){
			selectprevobj = jQuery('#taxdiv .taxchildren').eq(jQuery('#taxdiv .taxchildren').length-2); 
			hideval = jQuery(selectprevobj).val(); 
		}
	}else{
		hideval = -1;
	} 
	jQuery('#categoryhide').val(hideval);
}