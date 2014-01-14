<div id="chatContainer">
    <!-- top -->
    <div id="chatTopBar" class="rounded">
        <span>
            <img id="custom_chat_vagar" src="<?php echo $picture; ?>" width="23" height="23">
            <span class="name" id="custom_chat_name"><?php echo $GLOBALS['user']->name;?></span>
            <!-- <a href="" class="logoutButton rounded">Logout</a> -->
        </span>
    </div>
    
    <!-- chat room -->
    <div id="chatLineHolder" style="overflow: hidden; padding: 0px; width: 300px;">
            <div class="jspContainer" style="width: 350px; height: 300px;">
                <div class="jspPane" style="padding: 0px; top: -14px; width: 300px;">
                    
                </div>
            <!-- <div class="jspVerticalBar"><div class="jspCap jspCapTop"></div><div class="jspTrack" style="height: 360px;"><div class="jspDrag" style="height: 12px; top: 348px;"><div class="jspDragTop"></div><div class="jspDragBottom"></div></div></div><div class="jspCap jspCapBottom"></div></div> -->
        </div>
    </div>

    <!-- online user -->
    <div class="rounded" id="chatUsers">
        <?php print $onlineuser; ?>
    </div>

    <!-- send message -->
    <div class="rounded" id="chatBottomBar">
        <div class="tip"></div>
        <!-- <form action="" method="post" id="loginForm" style="display: none;">
            <input maxlength="16" class="rounded" name="name" id="name">
            <input class="rounded" name="email" id="email">
            <input type="submit" value="Login" class="blueButton">
        </form> -->
        <form action="" method="post" id="submitForm" style="display: block;">
            <input maxlength="255" class="rounded" name="chatText" id="chatText">
            <input type="hidden" name="cid" id="chatcid" value="<?php echo $cid; ?>">
            <input type="hidden" name="lastcmid" id="lastcmid" value="<?php echo $lastcmid; ?>">
            <input type="submit" value="发  送" class="blueButton" id="sendMsgButton">
        </form>
    </div>
    
</div>