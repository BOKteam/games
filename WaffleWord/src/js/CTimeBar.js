function CTimeBar(iX,iY){
    var _iX;
    var _iY;
    var _iHeight;
    var _iMaskWidth;
    var _oTimeText;
    var _oMask;
    var _oContainer;
    
    this._init = function(iX,iY){
       _iX = iX;
       _iY = iY;
       _oContainer = new createjs.Container();
       _oContainer.x = iX;
       _oContainer.y = iY;
       s_oStage.addChild(_oContainer); 
       
       var oSprite = s_oSpriteLibrary.getSprite('timebar_bg');
       var oBg = new createjs.Bitmap(oSprite);
       _oContainer.addChild(oBg);
       
       var oFill  = new createjs.Bitmap(s_oSpriteLibrary.getSprite('timebar_fill'));
       _oContainer.addChild(oFill);
       
       _iMaskWidth = oSprite.width;
       _oMask = new createjs.Shape();
       _oMask.graphics.beginFill("rgba(255,0,0,0.01)").drawRect(0, 0, oSprite.width,oSprite.height);
       _oContainer.addChild(_oMask);
       
       oFill.mask = _oMask;
       
       _oTimeText = new createjs.Text("","20px dimboregular", "#fff");
       _oTimeText.x = oSprite.width/2;
       _oTimeText.y = 22;
       _oTimeText.shadow = new createjs.Shadow("#000", 2, 2, 2);
       _oTimeText.textAlign = "center";
       _oTimeText.textBaseline = "alphabetic";
       _oContainer.addChild(_oTimeText);
       
       _iHeight = oSprite.height;
    };
    
    this.refreshTime= function(iTime){
        _oTimeText.text = formatTime(Math.floor(iTime));
			
	var iPerc = (iTime/TIME_LEVEL).toFixed(2);
	_oMask.scaleX = iPerc;
    };
    
    this._init(iX,iY);
}