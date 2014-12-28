/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.ProgressBar");
goog.require("org.createjs.easeljs.EaselJS");

quiz.features.game.v.component.ProgressBar = ProgressBar;
BOK.inherits(ProgressBar, createjs.Container);

function ProgressBar() {
	createjs.Container.call(this);
	
	var bg_ = new createjs.Shape(new createjs.Graphics().beginFill("#040404").drawRoundRect(0, 0, CONST.PROGRESS_BAR.WIDTH, CONST.PROGRESS_BAR.HEIGHT, 50));
   this.addChild(bg_);

   // the height of per bar
	this.perHeight_ = (CONST.PROGRESS_BAR.HEIGHT - CONST.PROGRESS_BAR.PER_OFFSET * 2) / CONST.PROGRESS_BAR.QUIZ_NUMBER;
   // the arrays contains all bars 
   this.quizMaxIndex_ = CONST.PROGRESS_BAR.QUIZ_NUMBER - 1;
}

ProgressBar.prototype.addOneBar = function(index, isCorrect){
   var correct = null;
	var correctG = null;
	var fillColor = isCorrect ? "#1CBD79" : "#F0535A";
	var i = this.quizMaxIndex_- index;

	if(i == this.quizMaxIndex_){
      correctG = new createjs.Graphics().beginFill(fillColor).drawRoundRect(CONST.PROGRESS_BAR.PER_OFFSET, CONST.PROGRESS_BAR.PER_OFFSET + (this.perHeight_ * i), CONST.PROGRESS_BAR.WIDTH - CONST.PROGRESS_BAR.PER_OFFSET * 2, this.perHeight_ , 25).endFill();
      correct = new createjs.Shape(correctG);
	}else{
      correctG = new createjs.Graphics().beginFill(fillColor).drawRoundRect(CONST.PROGRESS_BAR.PER_OFFSET, CONST.PROGRESS_BAR.PER_OFFSET + (this.perHeight_ * i), CONST.PROGRESS_BAR.WIDTH - CONST.PROGRESS_BAR.PER_OFFSET * 2, this.perHeight_ + 15, 25).endFill();
      correct = new createjs.Shape(correctG);
	}
	
	correct.visible = true;

	if (i < this.quizMaxIndex_){
		this.addChildAt(correct, this.getChildIndex(this.barUnits_[index - 1].shape));	
	}else{
		this.addChild(correct);
	}
	correct.alpha = 0;
	createjs.Tween.get(correct).to({alpha: 1},500);
	this.barUnits_.push({shape: correct, graphics:correctG});
};

ProgressBar.prototype.init = function(){
	if(this.barUnits_){
		BOK.each(this.barUnits_, function(bar){
			this.removeChild(bar.shape);
		}, this);
	}
	this.barUnits_ = [];
};
