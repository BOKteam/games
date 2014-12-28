/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午9:08
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("quiz.features.game.v.GameViewMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("org.createjs.easeljs.EaselJS");
goog.requireAll("quiz.features.game.v.component.*");
goog.require("quiz.features.game.MainGameFeatureNotes");


quiz.features.game.v.GameViewMediator = GameViewMediator;
BOK.inherits(GameViewMediator, BaseMediator);

function GameViewMediator(stage) {
    BaseMediator.call(this);

    this.ui_ = new quiz.features.game.v.component.GameUI();
    this.ui_.hide();
    stage.addChild(this.ui_);
    
    //regist option event 
    BOK.each(this.ui_.options_, function(option){
    	option.addEventListener('click', Delegate.create(this, this.onOptionBtnClick_, option));
    }, this);
    this.tickListener_ = createjs.Ticker.addEventListener('tick', Delegate.create(this, this.update_));
}
GameViewMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(MainGameFeatureNotes.getInternalNote('START_GAME'), this.onGameStart_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GO_GAME'), this.onGameInit_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('CHECK_ANSWER_RESULT'), this.onCheckAnswerResult_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GET_RESULT'), this.onGetResult_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('NEXT_QUESTION_INFO'), this.onGoNextQuestion_);
    GameViewMediator.superClass_.declareInterestedNotifications.call(this);
};
GameViewMediator.prototype.onOptionBtnClick_ = function(option){
    var optionIndex = option.getOption();
    if(this.answers_[this.currentIndex_]){
        return;
    }

    option.setChosenColor();
    this.answers_.push(optionIndex);
    this.sendNotification(MainGameFeatureNotes.getInternalNote('SELECT_OPTION'), {questionIndex: this.currentIndex_, option: optionIndex, time: new Date().getTime() - this.startQuizTime_});
};

GameViewMediator.prototype.update_ = function(){
    if(this.ui_.timeBar_.isTimeOut()){
        if(this.answers_[this.currentIndex_]){
            this.ui_.timeBar_.stopAnimationTimeOut();
            return;
        }else{
            this.answers_.push(-1);
            this.sendNotification(MainGameFeatureNotes.getInternalNote('SELECT_OPTION'), {questionIndex: this.currentIndex_, option: -1});
        }
        this.ui_.timeBar_.restoreTime();
    }
};

GameViewMediator.prototype.onGetResult_ = function(e){
     this.ui_.hide();
     this.ui_.timeBar_.stopAnimationTimeOut();
     this.sendNotification(MainGameFeatureNotes.getInternalNote('FINISH_GAME'), {grade: e.body.grade, result: e.body.result, playerSides: this.playerSides_ }); 
};
GameViewMediator.prototype.onCheckAnswerResult_ = function(e){
    var answer = e.body.answer;
    var correctAnswer = e.body.correctAnswer;
    var playerName = e.body.playerName;
    var result = e.body.result;
    var score = e.body.score;
    if(this.player_.name == playerName){
        if(result){
            this.score_ += score;
            this.ui_.updateMeScore(this.score_, score);
        }
        this.ui_.processBarL_.addOneBar(this.currentIndex_, result);
        this.showAnswer_(answer, correctAnswer);
        if(this.currentIndex_ == this.questions.length - 1){
            var self = this;
            setTimeout(function(){
                 self.sendNotification(MainGameFeatureNotes.getInternalNote('FINISH_SELECT'), {oppName: self.oppName_, score:self.score_});
            }, 1000);

        }
    }else{
        if(result){
            this.oppScore_ += score;
            this.ui_.updateRScore(this.oppScore_, score);
        }
        this.ui_.processBarR_.addOneBar(this.matchIndex_, result);
        if(this.matchIndex_ != this.questions.length - 1){
            this.matchIndex_ ++;
        }
    }
};

GameViewMediator.prototype.onGoNextQuestion_ = function(e){
    this.ui_.timeBar_.setDuration(e.body.duration / 1000);
    this.ui_.timeBar_.stopAnimationTimeOut();
    var self = this;
    setTimeout(function(){
        self.ui_.timeBar_.startAnimationTimeOut_();
        self.goNextQuestion_();
     }, 1000);
    
};

GameViewMediator.prototype.checkAnswer_ = function(){
     return this.questions[this.currentIndex_].correctAnswer == this.answers[this.currentIndex_];
};
GameViewMediator.prototype.onGameInit_ = function(e){
    this.ui_.init();
    this.player_ = e.body.player;
    var playerSides = BOK.cloneObject(e.body.playerSides);
    if(playerSides[0].name != this.player_.name) {
        playerSides.push(playerSides.shift());
    }
    this.oppName_ = playerSides[1].name;
    this.playerSides_ = playerSides;
    this.ui_.setPlayersInfo(this.playerSides_);
    this.questions = e.body.questions;
    this.currentIndex_ = 0;
    this.matchIndex_ = 0;
    this.score_ = 0;
    this.oppScore_ = 0;
    this.answers_ = [];
    this.setQuestionInfo_();
    this.ui_.timeBar_.startAnimationTimeOut_();

};

GameViewMediator.prototype.setQuestionInfo_ = function (){
    var question = this.questions[this.currentIndex_];
    this.ui_.setQuestion(question);
    this.ui_.timeBar_.restoreTime();
    this.startQuizTime_ = new Date().getTime();
};
GameViewMediator.prototype.goNextQuestion_ = function(){
    this.currentIndex_ ++;
    this.setQuestionInfo_();
    
};

GameViewMediator.prototype.showAnswer_ = function(answer, correctAnswer){
    this.ui_.options_[correctAnswer].showAnswerStatus(true);
    if((answer != correctAnswer) && (answer != -1)){
        this.ui_.options_[answer].showAnswerStatus(false);
    }
};
