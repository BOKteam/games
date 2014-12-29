function CEndPanel(){
    var _oOppScore;
    var _oPointText;
    var _oPointText2;
    var _oNumWordFoundText;
    var _oLongestWordText;
    var _oBestWordText;
    var _oButExit;
    var _oButReplay;
    var _oGroup;
    var _oResult;
    var oppName = "";
    this._init = function(){
        
        var oBg = new createjs.Bitmap(s_oSpriteLibrary.getSprite('gameover_bg'));
        
        var oTotScore = new createjs.Text(TEXT_TOT_SCORE,"50px dimboregular", "#FFCC00");
        oTotScore.x = 170;
        oTotScore.y = 200; 
        oTotScore.textAlign = "left";
        oTotScore.textBaseline = "alphabetic";
        oTotScore.shadow = new createjs.Shadow("#000", 2, 2, 2);

        //var oppName = "";
        if(s_oMain.name == "host"){
            if(!s_isSinglePlayer){
                oppName = s_oMain.playerInfoArray_[1].name+'@'+s_oMain.playerInfoArray_[1].seat;
            }
        }else {
            oppName = s_oMain.playerInfoArray_[0].name+'@'+s_oMain.playerInfoArray_[0].seat;
        }
        _oOppScore = new createjs.Text(oppName + " " + TEXT_SCORE,"50px dimboregular", "#FFCC00");
        _oOppScore.x = 170;
        _oOppScore.y = 270;
        _oOppScore.textAlign = "left";
        _oOppScore.textBaseline = "alphabetic";
        _oOppScore.shadow = new createjs.Shadow("#000", 2, 2, 2);
        
        _oPointText = new createjs.Text("0","50px dimboregular ", "#fff");
        _oPointText.x = 500;
        _oPointText.y = 200; 
        _oPointText.textAlign = "left";
        _oPointText.textBaseline = "alphabetic";
        _oPointText.shadow = new createjs.Shadow("#000", 2, 2, 2);

        _oPointText2 = new createjs.Text("0","50px dimboregular", "#fff");
        _oPointText2.x = 500;
        _oPointText2.y = 270;
        _oPointText2.textAlign = "left";
        _oPointText2.textBaseline = "alphabetic";
        _oPointText2.shadow = new createjs.Shadow("#000", 2, 2, 2);
        
        var oWordFoundText = new createjs.Text(TEXT_WORD_FOUND,"40px dimboregular", "#FFCC00");
        oWordFoundText.x = 200;
        oWordFoundText.y = 350; 
        oWordFoundText.textAlign = "left";
        oWordFoundText.textBaseline = "alphabetic";
        oWordFoundText.shadow = new createjs.Shadow("#000", 2, 2, 2);
        
        _oNumWordFoundText = new createjs.Text("0","40px dimboregular", "#fff");
        _oNumWordFoundText.x = 450;
        _oNumWordFoundText.y = 350; 
        _oNumWordFoundText.textAlign = "left";
        _oNumWordFoundText.textBaseline = "alphabetic";
        _oNumWordFoundText.shadow = new createjs.Shadow("#000", 2, 2, 2);
        
        var oLongestText = new createjs.Text(TEXT_LONGEST_WORD,"40px dimboregular", "#FFCC00");
        oLongestText.x = 200;
        oLongestText.y = 400; 
        oLongestText.textAlign = "left";
        oLongestText.textBaseline = "alphabetic";
        oLongestText.shadow = new createjs.Shadow("#000", 2, 2, 2);
        
        _oLongestWordText = new createjs.Text("WORD","40px dimboregular", "#fff");
        _oLongestWordText.x = 450;
        _oLongestWordText.y = 400; 
        _oLongestWordText.textAlign = "left";
        _oLongestWordText.textBaseline = "alphabetic";
        _oLongestWordText.shadow = new createjs.Shadow("#000", 2, 2, 2);
        
        var oBestText  = new createjs.Text(TEXT_BEST_WORD,"40px dimboregular", "#FFCC00");
        oBestText.x = 200;
        oBestText.y = 450; 
        oBestText.textAlign = "left";
        oBestText.textBaseline = "alphabetic";
        oBestText.shadow = new createjs.Shadow("#000", 2, 2, 2);
        
        _oBestWordText = new createjs.Text("WORD","40px dimboregular", "#fff");
        _oBestWordText.x = 450;
        _oBestWordText.y = 450; 
        _oBestWordText.textAlign = "left";
        _oBestWordText.textBaseline = "alphabetic";
        _oBestWordText.shadow = new createjs.Shadow("#000", 2, 2, 2);

        _oResult = new createjs.Text("","64px dimboregular", "#FFCC00");
        _oResult.x = 305;
        _oResult.y = 520;
        _oResult.textAlign = "left";
        _oResult.textBaseline = "alphabetic";
        _oResult.shadow = new createjs.Shadow("#000", 2, 2, 2);
        
        var oSprite = s_oSpriteLibrary.getSprite('but_play');
        _oButExit = new CTextButton((CANVAS_WIDTH/2) - 10,CANVAS_HEIGHT - 160,oSprite,TEXT_EXIT,"dimboregular","#ffffff",52,false);
        _oButExit.addEventListener(ON_MOUSE_UP, this._onExit, this);
        _oButReplay = new CTextButton((CANVAS_WIDTH/2) - 310,CANVAS_HEIGHT - 160,oSprite,'REPLAY',"dimboregular","#ffffff",52,false);
        _oButReplay.addEventListener(ON_MOUSE_UP, this._onReplay, this);

        _oGroup = new createjs.Container();
        _oGroup.alpha = 0;
        _oGroup.visible=false;
        
        _oGroup.addChild(oBg,oTotScore,_oOppScore,_oPointText,_oPointText2,oWordFoundText,_oNumWordFoundText,oLongestText,_oLongestWordText,oBestText,_oBestWordText,_oButExit.getSprite(),_oButReplay.getSprite(),_oResult);

        _oOppScore.visible = !s_isSinglePlayer;
        _oPointText2.visible = !s_isSinglePlayer;
        _oResult.visible = !s_isSinglePlayer;

        s_oStage.addChild(_oGroup);
    };
    
    this.unload = function(){
        _oButExit.unload();
    };
    this.show = function(iScore,oppScore,iNumWords,szLongest,szBestWord,iBestScore){
        _oPointText.text=""+iScore;
        if(oppScore == null){
            _oGroup.removeChild(_oOppScore,_oPointText2)
        }else{
            _oPointText2.text=""+oppScore;
        }
        _oNumWordFoundText.text=""+iNumWords;
        _oLongestWordText.text=szLongest;
        _oBestWordText.text=szBestWord +" +"+iBestScore;
        _oResult.text = ""+(iScore > oppScore ? TEXT_RESULT_WIN : (iScore < oppScore ? TEXT_RESULT_LOSS : TEXT_RESULT_TIE));

        _oGroup.visible = true;

        createjs.Tween.get(_oGroup).to({alpha:1 }, 500);
        
        $(s_oMain).trigger("save_score",iScore);		
    };
    
    this._onExit = function(){
        s_oGame.onExit(false);
    };
    
    this._onReplay = function(){
        s_oGame.onExit(true, s_isSinglePlayer ? null : {vsInfo: s_oMain.playerInfoArray_, oppName: oppName});
    };

    this._init();
    
    return this;
}
