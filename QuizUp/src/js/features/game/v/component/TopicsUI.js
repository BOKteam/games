/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.TopicsUI");
goog.require("org.createjs.easeljs.EaselJS");
goog.require("bok.easelui.SosaIcon");
goog.require("bok.util.EaselAnimationHelper");
goog.require("quiz.features.game.v.component.TopicBar");
goog.require('quiz.AssetsList');

quiz.features.game.v.component.TopicsUI = TopicsUI;
BOK.inherits(TopicsUI, createjs.Container);

function TopicsUI (){
	createjs.Container.call(this);
	this.blocked = false;
	this.topics_ = null;
	this.topicBars_ = [];
	this.topicsPanel_ = new createjs.Container();
	this.bg_ = new createjs.Shape(new createjs.Graphics().beginFill("#00B3D8").drawRect(0, 0, CONST.BG.WIDTH, CONST.BG.HEIGHT));
    this.addChild(this.bg_);
    this.addChild(this.topicsPanel_);
    this.title_ = new createjs.Text('Topics', '30px ' + CONST.FONT.ALL, '#fff');
	this.title_.set(TopicsUI.settings.title);
	this.addChild(this.title_);
	this.msgPanel_ = new createjs.Container();
    this.readyTime_ = new createjs.Text("", "18px " + CONST.FONT.ALL, "#FFFFFF");

    this.readyTime_.set({ x: (CONST.TOPIC_UI.MATCH_PANEL_WIDTH ) / 2, y: (CONST.TOPIC_UI.MATCH_PANEL_HEIGHT) * 3 / 4, textAlign:'center'});

	this.msgPanel_.addChild(new createjs.Shape(new createjs.Graphics().beginFill("rgba(0,0,0,0.8)").drawRoundRect(0, 0, CONST.TOPIC_UI.MATCH_PANEL_WIDTH, CONST.TOPIC_UI.MATCH_PANEL_HEIGHT, 6).endFill()), this.readyTime_);
	this.icon_ = 
	this.msgIcon_ = new SosaIcon('[', CONST.TOPIC_UI.MATCH_ICON_SIZE, {border: 0,fontColor:'#fff', bgColor: 'rgba(255,255,255,0)'});
	this.msgIcon_.set({x: (CONST.TOPIC_UI.MATCH_PANEL_WIDTH - CONST.TOPIC_UI.MATCH_ICON_SIZE) / 2, y: (CONST.TOPIC_UI.MATCH_PANEL_HEIGHT- CONST.TOPIC_UI.MATCH_ICON_SIZE) / 4})
	this.msgPanel_.addChild(this.msgIcon_);
	this.msgPanel_.set(TopicsUI.settings.msg);
	this.addChild( this.msgPanel_);

	this.prev_ = new SosaIcon("R", CONST.TOPIC_UI.PAGE_SIZE, {border: 0, bgColor: '#00B3D8', fontColor: '#fff'});
	this.next_ = new SosaIcon("Q", CONST.TOPIC_UI.PAGE_SIZE, {border: 0, bgColor: '#00B3D8', fontColor: '#fff'});
	this.page_ = new createjs.Text('1/1', "50px " + CONST.FONT.ALL, "#fff" );
	this.page_.visible =false;
	this.page_.set(TopicsUI.settings.page)
	this.prev_.set(TopicsUI.settings.prev);
	this.next_.set(TopicsUI.settings.next);
	this.addChild(this.prev_, this.next_, this.page_);



	this.footer_ = new createjs.Container();
	this.footer_.addChild(new createjs.Shape(new createjs.Graphics().beginFill('#0A0A0A').drawRect(0,0,CONST.BG.WIDTH, CONST.TOPIC_UI.FOOTER_HEIGHT).endFill()));
	this.footer_.set(TopicsUI.settings.footer);
	this.history_ = new createjs.Shape();
	this.footerBar_ = {history: this.generateFootBar_('\u0120', 'History',CONST.TOPIC_UI.FOOTER_BAR_WIDTH, CONST.TOPIC_UI.FOOTER_HEIGHT)};

	BOK.each(this.footerBar_, function(profile){
		profile.set({x: CONST.BG.WIDTH / 2 - CONST.TOPIC_UI.FOOTER_BAR_WIDTH / 2});
		this.footer_.addChild(profile);
	},this);

	this.addChild(this.footer_);
	this.addChild(this.history_);
}
TopicsUI.prototype.setTitle = function(title){
	this.title_.text = title;
};
TopicsUI.prototype.setPage = function(currentPage, totalPage){
		this.page_.text  = currentPage + '/' + totalPage;
		this.page_.visible = true;
};

TopicsUI.prototype.generateFootBar_ = function(icode, text, width, height){
	var footBar = new createjs.Container();
	var text = new createjs.Text(text, CONST.TOPIC_UI.FOOT_BAR_SIZE / 2 + 'px ' + CONST.FONT.ALL, '#9873E6');
	var icon = new SosaIcon(icode, CONST.TOPIC_UI.FOOT_BAR_SIZE, {border: 0, bgColor: '#000', fontColor: '#9873E6'});

	icon.set({x:width / 2 - CONST.TOPIC_UI.FOOT_BAR_SIZE / 2 , y: height / 4 - CONST.TOPIC_UI.FOOT_BAR_SIZE / 2})
	text.set({x:width / 2 , y: height * 3/ 4 - CONST.TOPIC_UI.FOOT_BAR_SIZE / 4, textAlign:'center', textBaseline: 'middle'})
	footBar.addChild(text, icon);
	return footBar;
};

TopicsUI.settings = {
	next:{x: CONST.BG.WIDTH * 2/ 3 - CONST.TOPIC_UI.PAGE_SIZE/ 2, y: CONST.BG.HEIGHT * 7/ 10},
	prev:{x: CONST.BG.WIDTH * 1/ 3 - CONST.TOPIC_UI.PAGE_SIZE/ 2, y: CONST.BG.HEIGHT * 7/ 10},
	footer:{x: 0, y: CONST.BG.HEIGHT - CONST.TOPIC_UI.FOOTER_HEIGHT},
	page:{x: CONST.BG.WIDTH / 2, y:CONST.BG.HEIGHT * 7/ 10 + CONST.TOPIC_UI.PAGE_SIZE/ 2, textAlign: 'center', textBaseline: 'middle'},
	msg: {visible:false,x: (CONST.BG.WIDTH - CONST.TOPIC_UI.MATCH_PANEL_WIDTH) / 2, y: (CONST.BG.HEIGHT -  CONST.TOPIC_UI.MATCH_PANEL_HEIGHT) / 2},
	title: {x: CONST.BG.WIDTH / 2,y: CONST.BAR.HEIGHT / 2 , textAlign: 'center', textBaseline: 'middle'}
};

TopicsUI.prototype.initTopicsAll = function(topics){
	var count = topics.length;
	this.topicsPanel_.removeAllChildren();
	BOK.each(this.topicBars_, function(item){
		item.removeAllEventListeners();
	});
	this.topicBars_ = [];
	var i =0;

    BOK.each(topics, function(topic){
        var topicBar = new quiz.features.game.v.component.TopicBar(topic.topic, TopicsUI.icon[topic.topic.toLowerCase()] ||TopicsUI.icon.defaultValue);
		topicBar.set({x: (CONST.BG.WIDTH - CONST.TOPIC_BAR.WIDTH) / 2, y: ( CONST.BG.HEIGHT - CONST.TOPIC_BAR.HEIGHT * count - CONST.TOPIC_UI.TOPIC_BAR_SPACE * count) *4/ 10  + CONST.TOPIC_BAR.HEIGHT * i + CONST.TOPIC_UI.TOPIC_BAR_SPACE * i});
        this.topicsPanel_.addChild(topicBar);
        this.topicBars_.push({name:topic.topic.toLowerCase(), bar:topicBar});
        EaselAnimationHelper.standOut(topicBar.bg_, 3, 600, i*200);
        i ++;
    }, this);
};

TopicsUI.prototype.getTopicBarByName = function(name){
	var resultBar = null;
	BOK.each(this.topicBars_, function(topicBar){
	console.log(topicBar.name == name);
		if(topicBar.name == name){
			resultBar = topicBar.bar;
		}
	}, this)
	return resultBar;
};

TopicsUI.icon = {
	history: '\u0127',
	science: '\u0124',
	geography: '\u0125',
	literature: '\u3003',
	sport: 'L',
	defaultValue: 'f'
};

TopicsUI.prototype.showNetworkWaiting = function(){
	this.msgPanel_.y = 0 - TopicsUI.settings.msg.y;
	this.msgPanel_.visible = true;
	EaselAnimationHelper.moveTo(this.msgPanel_, this.msgPanel_.x, TopicsUI.settings.msg.y, CONST.TIME.HISTORY_SWITCH, createjs.Ease.elasticOut);
    this.blocked = true;	
};

TopicsUI.prototype.show = function(){
	this.x = this.x - CONST.BG.WIDTH;
	this.visible = true;	
	this.msgPanel_.visible = false;
	EaselAnimationHelper.moveTo(this, 0, this.y, CONST.TIME.SWITCH);
	this.resetTopic();
};
TopicsUI.prototype.resetTopic = function(){
	BOK.each(this.topicBars_, function(topicBar){
		topicBar.bar.reset();
    });
};
TopicsUI.prototype.hide = function(){
	EaselAnimationHelper.moveTo(this, 0 - CONST.BG.WIDTH, this.y, CONST.TIME.SWITCH).call(Delegate.create(this,function(){
		this.visible = false;	
		this.x = 0;
	}));	
	this.blocked = false;
};

TopicsUI.prototype.setReadydDuration = function(duration) {
    this.readyTime_.text = "Waiting for opponent:" + duration;
};
