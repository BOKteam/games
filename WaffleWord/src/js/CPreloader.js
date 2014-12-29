function CPreloader(){
    var _oLoadingText;
    var _oContainer;
    
    this._init = function(){
       _oContainer = new createjs.Container();
       s_oStage.addChild(_oContainer);
       
       _oLoadingText = new createjs.Text("","bold 22px latin_xcn_btregular center", "#ffffff");
       _oLoadingText.x = (CANVAS_WIDTH/2)-40;
       _oLoadingText.y = CANVAS_HEIGHT/2 + 200;
       _oContainer.addChild(_oLoadingText);
    };
    
    this.unload = function(){
	_oContainer.removeAllChildren();
    };
    
    this.refreshLoader = function(iPerc){
        _oLoadingText.text = iPerc+"%";
    };
    
    this._init();   
}
