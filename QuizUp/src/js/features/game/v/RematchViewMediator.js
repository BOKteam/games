/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午9:08
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("quiz.features.game.v.RematchViewMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("org.createjs.easeljs.EaselJS");
goog.require("quiz.features.game.v.GamePlayMediator");
goog.requireAll("quiz.features.game.v.component.*");
goog.require("quiz.features.game.MainGameFeatureNotes");


quiz.features.game.v.RematchViewMediator = RematchViewMediator;
BOK.inherits(RematchViewMediator, BaseMediator);

function RematchViewMediator(stage) {
	BaseMediator.call(this);
	this.ui_ = new quiz.features.game.v.component.RematchTopicsUI();
    this.ui_.setTitle('Rematch');
	this.ui_.visible = false;
    this.ui_.removeChild(this.ui_.footer_);
	stage.addChild(this.ui_);
    this.meSeletTopic_ = null;
    this.oppSeletTopic_ = null;
	this.currentPage_  = 1;
    this.totalPage_ = 1;

    this.ui_.next_.addEventListener('click', Delegate.create(this, this.onNextClick_));
    this.ui_.prev_.addEventListener('click', Delegate.create(this, this.onPrevClick_));
}
RematchViewMediator.prototype.declareInterestedNotifications = function() {
 	this.declareInterest(MainGameFeatureNotes.getInternalNote('TOPIC_LIST_READY'), this.onTopicListReady_);
 	this.declareInterest(MainGameFeatureNotes.getInternalNote('GAME_INIT'), this.onGameInit_);
	this.declareInterest(MainGameFeatureNotes.getInternalNote('GO_REMATCH'), this.onGoRematch_);
	this.declareInterest(MainGameFeatureNotes.getInternalNote('REMATCH_TOPIC_RESULT'), this.onRematchTopicResult_);
    RematchViewMediator.superClass_.declareInterestedNotifications.call(this);
};

RematchViewMediator.prototype.onGoRematch_ = function(){
    this.init_();
	this.ui_.show();
};
RematchViewMediator.prototype.init_ = function(){
    this.meSeletTopic_ = null;
    this.oppSeletTopic_ = null;  
};
RematchViewMediator.prototype.onNextClick_ = function(e){
    if(this.currentPage_  >= this.totalPage_) {return false;}
    this.currentPage_ ++;
    this.sendNotification(MainGameFeatureNotes.getInternalNote('GET_TOPIC'), this.currentPage_);
};

RematchViewMediator.prototype.onPrevClick_ = function(e){
    if(this.currentPage_  <= 1) {return false;}
    this.currentPage_ --;
    this.sendNotification(MainGameFeatureNotes.getInternalNote('GET_TOPIC'), this.currentPage_);
};


RematchViewMediator.prototype.onTopicListReady_ = function(msg){
    this.ui_.initTopicsAll(msg.body.topicList);
    this.totalPage_ = Math.ceil(msg.body.count / 5 );
    this.ui_.setPage(this.currentPage_, this.totalPage_);
    BOK.each(this.ui_.topicBars_, function(topic){
        topic.bar.addEventListener('click', Delegate.create(this, this.onTopicBtnClick_, topic));
    }, this);
};

/** Event Listener */
RematchViewMediator.prototype.onTopicBtnClick_ = function(topic){
    if(this.ui_.blocked) return;
    this.meSeletTopic_ = topic.bar.getTopic().toLowerCase();
    BOK.each(this.ui_.topicBars_, function(topic){
        var name = topic.name;
        if((name != this.meSeletTopic_) && (name != this.oppSeletTopic_)){topic.bar.reset();}
    }, this);
    topic.bar.setPressedColor();
    this.sendNotification(MainGameFeatureNotes.getInternalNote('REMATCH_SELECT_TOPIC'), {topic: topic.bar.getTopic()});
};

RematchViewMediator.prototype.onRematchTopicResult_ = function(msg){
	var topic = msg.body.topic;	
	var result = msg.body.result;	
	var playerId = msg.body.id;	
	if(result){
		this.sendNotification(MainGameFeatureNotes.getInternalNote('SET_TOPIC'), topic);
    	this.sendNotification(MainGameFeatureNotes.getInternalNote('READY_TO_START'), {topic: topic});
	}
	if(playerId != appConfig.player.id){
        this.oppSeletTopic_ = topic.toLowerCase();
        BOK.each(this.ui_.topicBars_, function(topicBar){
            var name = topicBar.name;
            if((name != this.meSeletTopic_) && (name != this.oppSeletTopic_)){topicBar.bar.reset();}
        }, this);
        var oppSelectBar = this.ui_.getTopicBarByName(topic.toLowerCase());
        if(oppSelectBar){
             oppSelectBar.setRematchOppColor();
        }
	}
};

RematchViewMediator.prototype.onGameInit_ = function(){
    this.ui_.visible = false;
    this.ui_.hide();
};