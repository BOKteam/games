function CHelpPanel(){
    var _oText;
    var _oHelpBg;
    var _oButExit;
    var _oContainer;

    this._init = function(){
        _oHelpBg = new createjs.Bitmap(s_oSpriteLibrary.getSprite('help_bg')); 
		
	_oText = new createjs.Text(TEXT_HELP,"70px dimboregular", "#FFCC00");
        _oText.x = CANVAS_WIDTH/2;
        _oText.y = 150; 
        _oText.textAlign = "center";
        _oText.textBaseline = "alphabetic";
        _oText.lineHeight = 70;
        _oText.shadow = new createjs.Shadow("#000", 2, 2, 2);

        var oSprite = s_oSpriteLibrary.getSprite('but_play');
        _oButExit = new CTextButton((CANVAS_WIDTH/2),CANVAS_HEIGHT - 80,oSprite,TEXT_PLAY,"dimboregular","#ffffff",52,false);
        _oButExit.addEventListener(ON_MOUSE_UP, this._onExit, this); 

        _oContainer = new createjs.Container();
        _oContainer.addChild(_oHelpBg,_oText,_oButExit.getSprite());
        s_oStage.addChild(_oContainer);

    };

    this.unload = function(){
        _oButExit.unload();
        
        s_oStage.removeAllChildren();
    };

    this._onExit = function(){
        this.unload();
        s_oMain.gotoGame();
    };

    this._init();

}
