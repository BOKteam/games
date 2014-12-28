/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.TopicBar");
goog.require("org.createjs.easeljs.EaselJS");
goog.require("bok.easelui.SosaIcon");


quiz.features.game.v.component.TopicBar = TopicBar;
BOK.inherits(TopicBar, createjs.Container);

/**
 * the topic bar
 * @param {string} topic // the topic name
 */
function TopicBar (topic, iconCode){
	this.topic_ = topic;
	createjs.Container.call(this);
	this.bg_ = new createjs.Shape(new createjs.Graphics().beginFill("#FAFAFA").drawRoundRect(0, 0, CONST.TOPIC_BAR.WIDTH,CONST.TOPIC_BAR.HEIGHT, 4));
	this.addChild(this.bg_);
	this.topicText_ = new createjs.Text(topic , "25px " + CONST.FONT.ALL, "#000000");
	this.topicText_.set({x: CONST.TOPIC_BAR.ICON_SPACE, y: CONST.TOPIC_BAR.HEIGHT / 2 , textAlign: "left", textBaseline: "middle", maxWidth: CONST.TOPIC_BAR.WIDTH - CONST.TOPIC_BAR.WIDTH / 2});
	this.point_ = new SosaIcon("\u203A", CONST.TOPIC_BAR.POINT_WIDTH, {border: 0});
	this.point_.set({x: this.topicText_.x + this.topicText_.maxWidth + (CONST.TOPIC_BAR.WIDTH - this.topicText_.x - this.topicText_.maxWidth) / 2, y: (CONST.TOPIC_BAR.HEIGHT -CONST.TOPIC_BAR.POINT_WIDTH )/ 2 - 5 });
	if(iconCode){
		this.icon_ = new SosaIcon(iconCode, CONST.TOPIC_BAR.POINT_WIDTH, {border: 0});
		this.icon_.set({x: 10, y: (CONST.TOPIC_BAR.HEIGHT -CONST.TOPIC_BAR.POINT_WIDTH )/ 2 - 5 });
		this.addChild(this.icon_)
	}
	
	this.addChild(this.topicText_, this.point_);
}

TopicBar.prototype.setPressedColor = function(){
	var graphics = new createjs.Graphics().beginFill("#555555").drawRoundRect(0, 0, CONST.TOPIC_BAR.WIDTH,CONST.TOPIC_BAR.HEIGHT, 4);
    this.bg_.graphics = graphics;
};

TopicBar.prototype.setRematchOppColor = function(){
	var graphics = new createjs.Graphics().beginFill("#FFA026").drawRoundRect(0, 0, CONST.TOPIC_BAR.WIDTH,CONST.TOPIC_BAR.HEIGHT, 4);
    this.bg_.graphics = graphics;
};

TopicBar.prototype.reset = function(){
    var graphics = new createjs.Graphics().beginFill("#FAFAFA").drawRoundRect(0, 0, CONST.TOPIC_BAR.WIDTH,CONST.TOPIC_BAR.HEIGHT, 4);
    this.bg_.graphics = graphics;
};

TopicBar.prototype.getTopic = function(){
	return this.topic_;
};
