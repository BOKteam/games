/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.TimeBar");
goog.require("org.createjs.easeljs.EaselJS");

quiz.features.game.v.component.TimeBar = TimeBar;
BOK.inherits(TimeBar, createjs.Container);

function TimeBar(){
	createjs.Container.call(this);
	this.inShape_ = new createjs.Shape(new createjs.Graphics().beginFill("#010101").drawCircle(0 , 0, CONST.TIME_BAR.IN_RADIUS).endFill());

	this.outShape_ = new createjs.Shape(new createjs.Graphics().beginFill("#28B4D4").drawCircle(0 , 0, CONST.TIME_BAR.OUT_RADIUS).endFill());

	this.durationText_ = new createjs.Text(CONST.TIME_BAR.DURATION,"20px " + CONST.FONT.ALL, "#0AA7CA");
	this.durationText_.set({x: 0, y: CONST.TIME_BAR.IN_RADIUS / 3 , textAlign: "center" ,textBaseline: "center"});

	this.finishedTime_ = new createjs.Shape();
	this.addChild(this.outShape_, this.finishedTime_, this.inShape_, this.durationText_);
}
TimeBar.prototype.init = function(){
	this.duration_ = CONST.TIME_BAR.DURATION;
	this.animateDuration_ = this.duration_ * 10;
	this.actionTime_ = 1;
	this.isTimeOut_ = false;
	this.durationText_.text = CONST.TIME_BAR.DURATION;
	this.restoreTime();
};

TimeBar.prototype.startAnimationTimeOut_ = function(){
	this.animateDuration_ --;
	if(this.animateDuration_ <= 1){
		this.isTimeOut_ = true;
	}else{
		this.finishedG_ = new createjs.Graphics().beginFill("#00313A").lineTo(0,0).arc (0,0, CONST.TIME_BAR.OUT_RADIUS, 3 * Math.PI / 2, 3 * Math.PI / 2 + (Math.PI / (5 * CONST.TIME_BAR.DURATION) * (CONST.TIME_BAR.DURATION * 10  - this.animateDuration_ )) , false);
	    this.finishedTime_.graphics = this.finishedG_;
	    this.actionTime_ ++;
	    if(this.actionTime_ >= 10 ){
	    	this.actionTime_ = 1;
	    	if(this.duration_ > 0){
				this.durationText_.text = this.duration_--;
	    	}
	    }
	}   
    this.animateTimeoutId_ = setTimeout(Delegate.create(this, this.startAnimationTimeOut_), 100);
};


TimeBar.prototype.restoreTime = function(){
    this.finishedTime_.graphics = null;
    this.actionTime_ = 1;
    this.duration_ = CONST.TIME_BAR.DURATION;
    this.animateDuration_ = CONST.TIME_BAR.DURATION * 10;
    this.durationText_.text = CONST.TIME_BAR.DURATION;
    this.isTimeOut_ = false;
};

TimeBar.prototype.stopAnimationTimeOut = function(){
	if(this.animateTimeoutId_){
		clearTimeout(this.animateTimeoutId_);
        this.animateTimeoutId_ = null;
	}
};

TimeBar.prototype.isTimeOut = function(){
	return this.isTimeOut_;	
};

TimeBar.prototype.setDuration = function(duration){
	this.duration_ = duration;
};
