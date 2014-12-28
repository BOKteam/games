function CLanguageSelection(){
    var _szLangSelected;
    var _oCurButSelected;
    var _oButPlay;
    var _oMsgTextBack;
    var _oMsgText;
    var _oContainer;
    
    this._init = function(){
        _oContainer = new createjs.Container();
        _oContainer.alpha = 0;
        _oContainer.visible=false;
        s_oStage.addChild(_oContainer);
        
        var oBg = new createjs.Bitmap(s_oSpriteLibrary.getSprite('bg_language_selection'));
        _oContainer.addChild(oBg);
        
        var oSprite = s_oSpriteLibrary.getSprite('but_play');
        _oButPlay = new CTextButton(CANVAS_WIDTH - oSprite.width/2 - 50,CANVAS_HEIGHT - 50,oSprite,TEXT_PLAY,"dimboregular","#ffffff",52,false);
        _oButPlay.addEventListener(ON_MOUSE_UP, this._onButPlayRelease, this);
        _oContainer.addChild(_oButPlay.getSprite());
        
        //INIT LANGUAGE TOGGLE
        var iYPos = 126;
        for(var i=0;i<NUM_LANGUAGES;i++){
            if(i === 0){
                var oLangToggle = new CToggle(50,iYPos,s_oSpriteLibrary.getSprite('toggle_lang'),true,false);
                _oCurButSelected = oLangToggle;
                _szLangSelected = window["LABEL_LANG_"+ i];
            }else{
                var oLangToggle = new CToggle(50,iYPos,s_oSpriteLibrary.getSprite('toggle_lang'),false,false);
            }
            
            oLangToggle.addEventListenerWithParams(ON_MOUSE_UP, this._onLangToggle, this,{index:i,but:oLangToggle});
            _oContainer.addChild(oLangToggle.getSprite());
            
            iYPos += 98;
        }

        _oMsgTextBack = new createjs.Text(TEXT_CHOOSE_LANG,"60px dimboregular", "#000");
        _oMsgTextBack.x = 22;
        _oMsgTextBack.y = 62; 
        _oMsgTextBack.textAlign = "left";
        _oMsgTextBack.textBaseline = "alphabetic";
        _oContainer.addChild(_oMsgTextBack);
        
        _oMsgText = new createjs.Text(TEXT_CHOOSE_LANG,"60px dimboregular", "#FFCC00");
        _oMsgText.x = 20;
        _oMsgText.y = 60; 
        _oMsgText.textAlign = "left";
        _oMsgText.textBaseline = "alphabetic";
        _oContainer.addChild(_oMsgText);
    };
    
    this.show = function(){
        _oContainer.visible = true;
        createjs.Tween.get(_oContainer).to({alpha:1}, 500);
    };
    
    this._onLangToggle = function(oRetParams){
        var oParam = oRetParams.params;

        if(oParam.but === _oCurButSelected){
            _oCurButSelected.setActive(true);
            return;
        }

        var iIndex = oParam.index;

        _oCurButSelected.setActive(false);
        _szLangSelected = window["LABEL_LANG_"+ iIndex];

        _oCurButSelected = oParam.but;
    };
    
    this._onButPlayRelease = function(){
        s_oMenu.unload();
        s_oMain.setLanguage(_szLangSelected);
    };
    
    this._init();
}