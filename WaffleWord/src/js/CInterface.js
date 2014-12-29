function CInterface(){
    var _aWordComposing;
    var _oButExit;
    var _oScoreText;
    var _oPointsText;
    var _oPointsText2;
    var _oWordWrittenText;
    var _oWordWrittenScoreText;
    var _oTimeBar;
    var _oAudioToggle;
    var _oHostAvatar;
    var _oSlaveAvatar;
    var _oNameText;

    this._init = function(){

	    _oScoreText = new createjs.Text(TEXT_SCORE,"30px dimboregular", "#000");
        _oScoreText.x = 730;
        _oScoreText.y = 108; 
        _oScoreText.rotation = -6;
        _oScoreText.textAlign = "center";
        _oScoreText.textBaseline = "alphabetic";
        s_oStage.addChild(_oScoreText);

        var infoAry = s_oMain.playerInfoArray_;
        var name = "";
        var avatar1 = "avator";
        var avatar2 = "avator";
        console.log(s_isSinglePlayer);
        if(s_isSinglePlayer){
            name = infoAry[0].name+'@'+infoAry[0].seat;
            avatar1 = infoAry[0].avatar;
        }else{
            if(s_oMain.name == "host"){
                name = infoAry[0].name+'@'+infoAry[0].seat + ' VS ' + infoAry[1].name+'@'+infoAry[1].seat;
                avatar1 = infoAry[0].avatar;
                avatar2 = infoAry[1].avatar;
            }else {
                name = infoAry[1].name+'@'+infoAry[1].seat + ' VS ' + infoAry[0].name+'@'+infoAry[0].seat;
                avatar1 = infoAry[1].avatar;
                avatar2 = infoAry[0].avatar;
            }
        }
        _oNameText = new createjs.Text(name,"25px dimboregular", "#000");
        _oNameText.x = 820;
        _oNameText.y = 45;
        _oNameText.textAlign = "center";
        _oNameText.textBaseline = "alphabetic";
        s_oStage.addChild(_oNameText);

        _oPointsText = new createjs.Text(":0","55px dimboregular", "#000");
        _oPointsText.x = 794;
        _oPointsText.y = 160;
        _oPointsText.textAlign = "center";
        _oPointsText.textBaseline = "alphabetic";

        _oPointsText2 = new createjs.Text(":0","55px dimboregular", "#000");
        _oPointsText2.x = 920;
        _oPointsText2.y = 160;
        _oPointsText2.textAlign = "center";
        _oPointsText2.textBaseline = "alphabetic";

        _oHostAvatar = new createjs.Bitmap(this.imageExist('../' +avatar1)==true ? ('../' +avatar1): s_oSpriteLibrary.getSprite("avatar4"));
        _oHostAvatar.set({x:714,y:115,scaleX:0.25, scaleY:0.25});
        _oSlaveAvatar = new createjs.Bitmap(this.imageExist('../' + avatar2)==true ? ('../' +avatar2): s_oSpriteLibrary.getSprite("avatar5"));
        _oSlaveAvatar.set({x:845,y:115,scaleX:0.25, scaleY:0.25});


        s_oStage.addChild(_oPointsText);
        s_oStage.addChild(_oPointsText2);
        s_oStage.addChild(_oHostAvatar);
        s_oStage.addChild(_oSlaveAvatar);

        _oSlaveAvatar.visible = !s_isSinglePlayer;
        _oPointsText2.visible = !s_isSinglePlayer;

        _aWordComposing = new Array();
        var iX = 110;
        for(var i=0;i<MAX_WORD_LENGTH;i++){
            var oText = new createjs.Text("","44px dimboregular", "#000");
            oText.x = iX;
            oText.y = 65; 
            oText.textAlign = "center";
            oText.textBaseline = "alphabetic";
            s_oStage.addChild(oText);
            
            iX += 74;
            
            _aWordComposing.push(oText);
        }
        
        _oWordWrittenText = new createjs.Text("","20px dimboregular", "#000");
        _oWordWrittenText.x = 750;
        _oWordWrittenText.y = 260; 
        _oWordWrittenText.textAlign = "left";
        _oWordWrittenText.textBaseline = "alphabetic";
        _oWordWrittenText.lineHeight = 20;
        s_oStage.addChild(_oWordWrittenText);
        
        _oWordWrittenScoreText = new createjs.Text("","20px dimboregular", "#000");
        _oWordWrittenScoreText.x = 880;
        _oWordWrittenScoreText.y = 260; 
        _oWordWrittenScoreText.textAlign = "left";
        _oWordWrittenScoreText.textBaseline = "alphabetic";
        _oWordWrittenScoreText.lineHeight = 20;
        s_oStage.addChild(_oWordWrittenScoreText);

        _oTimeBar = new CTimeBar(120,735);

        var oSprite = s_oSpriteLibrary.getSprite('but_exit');
        _oButExit = new CGfxButton(CANVAS_WIDTH - (oSprite.height/2) - 6,50 + (oSprite.height/2),oSprite,true);
        _oButExit.addEventListener(ON_MOUSE_UP, this._onExit, this);      
        
        if(DISABLE_SOUND_MOBILE === false || s_bMobile === false){
            var oSprite = s_oSpriteLibrary.getSprite('audio_icon');
            _oAudioToggle = new CToggle(CANVAS_WIDTH - (oSprite.height/2) -6,(oSprite.height/2) + 4,oSprite,s_bAudioActive);
            _oAudioToggle.addEventListener(ON_MOUSE_UP, this._onAudioToggle, this);
        }
    };

    this.unload = function(){
        _oButExit.unload();
        _oButExit = null;
        
        if(DISABLE_SOUND_MOBILE === false || s_bMobile === false){
            _oAudioToggle.unload();
            _oAudioToggle = null;
        }
    
    };

    this.refreshScore = function(iScore){
        _oPointsText.text = ":"+iScore;
    };

    this.refreshOpScore = function(opScore){
        _oPointsText2.text = ":"+opScore;
    };
    
    this.clearWordComposing = function(){
        for(var i=0;i<_aWordComposing.length;i++){
            _aWordComposing[i].text = "";
        }
    };
    
    this.setWordComposing = function(szText){
        for(var i=0;i<szText.length;i++){
            _aWordComposing[i].text = szText.charAt(i);
        }
    };
    
    this.displayAnswer = function(iScore){
        this.refreshScore(iScore);

        for(var i=0;i<MAX_WORD_LENGTH;i++){
            _aWordComposing[i].text="";
        }
    };

    this.displayOpAnswer = function(opScore){
        this.refreshOpScore(opScore);

        for(var i=0;i<MAX_WORD_LENGTH;i++){
            _aWordComposing[i].text="";
        }
    };
    
    this.refreshWordWritten = function(aWords){
        _oWordWrittenText.text="";
        _oWordWrittenScoreText.text="";

        var aTmp = new Array();


        var iNumSlice = (aWords.length - MAX_WORDS_WRITTEN_VIEWABLE);

        if(iNumSlice>0){
            aTmp = aWords.slice( iNumSlice,aWords.length);
        }else{
            aTmp=aWords;	
        }

        for(var i=0;i<aTmp.length;i++){
            _oWordWrittenText.text += aTmp[i].word +"\n";
            _oWordWrittenScoreText.text += "+"+aTmp[i].score+"\n";
        }
    };
    
    this._onAudioToggle = function(){
        createjs.Sound.setMute(s_bAudioActive);
        s_bAudioActive = !s_bAudioActive;
    };
    
    this._onExit = function(){
        s_oGame.onExit();  
    };
    
    this.updateTime = function(iTime){
        _oTimeBar.refreshTime(iTime);
    };
    this.imageExist = function(imgPath){
        var flag = false;
        $.ajax({
            url: imgPath,
            type: 'get',
            async: false,
            success: function(response){
                flag =  true;
            },
            statusCode:{
                404: function(){
                    flag = false;
                }
            }
        });
        return flag;
    };
    
    this._init();
    
    return this;
}
