/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.QuestionImgPanel");
goog.require("org.createjs.easeljs.EaselJS");

quiz.features.game.v.component.QuestionImgPanel = QuestionImgPanel;
BOK.inherits(QuestionImgPanel, createjs.Container);

function QuestionImgPanel(width , height, img){
	createjs.Container.call(this);
	this.width_ = width;
	this.height_ = height;
	this.bg_ = new createjs.Shape(new createjs.Graphics().beginFill("#F8F8F8").drawRoundRect(0, 0, width, height, 4).endStroke());

	this.addChild(this.bg_);
	this.setImage(img);
	
}

QuestionImgPanel.prototype.setImage = function(imgSrc){
	if(this.shape_){
		this.removeChild(this.shape_);
	}
	if(!imgSrc){
		this.visible = false;
		return;
	}

	var bitImg = new createjs.Bitmap(imgSrc);
	bitImg.set({});

	var img = new Image()
	img.src = imgSrc;
	img.onload = Delegate.create(this, function(){
		this.shape_ = new createjs.Shape(new createjs.Graphics().beginStroke(this.borderColor_).beginBitmapFill(img, "no-repeat", bitImg.getMatrix()).drawRoundRect(0, 0, this.width_, this.height_, 4).endStroke());
		this.addChild(this.shape_);
	});
	this.visible = true;
}

QuestionImgPanel.prototype.show = function(){
	this.visible = true;
};

QuestionImgPanel.prototype.hide = function(){
	this.visible = false;
};