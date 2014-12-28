/**
 * Created by Envee.
 *
 * Date: 14-11-18
 * Time: am 10:11
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide('quiz.features.game.v.component.HistoryUI');
goog.require('org.createjs.easeljs.EaselJS');
goog.require("bok.easelui.EsButton");
goog.require("bok.util.EaselAnimationHelper");
goog.require("quiz.features.game.v.component.Avatar");
goog.require('quiz.AssetsList');
goog.require("org.momentjs.Moment");

quiz.features.game.v.component.HistoryUI = HistoryUI;
BOK.inherits(HistoryUI, createjs.Container);

function HistoryUI(){
	createjs.Container.call(this);
	this.bg_ = new createjs.Shape(new createjs.Graphics().beginFill('#9470E0').beginLinearGradientFill(["#9470E0","#533F7D"], [0, 1], CONST.BG.WIDTH / 2,  CONST.HISTORY_UI.PANEL_HEIGHT + CONST.HISTORY_UI.PANEL_FILL_HEIGHT- 20, CONST.BG.WIDTH / 2, CONST.HISTORY_UI.PANEL_HEIGHT + CONST.HISTORY_UI.PANEL_FILL_HEIGHT ).drawRect(0, 0, CONST.BG.WIDTH,  CONST.HISTORY_UI.PANEL_HEIGHT + CONST.HISTORY_UI.PANEL_FILL_HEIGHT).endFill());
	this.addChild(this.bg_);
	this.historyBars_ = [];
	this.historyListPanel_ = new createjs.Container();
	this.addChild(this.historyListPanel_);
	this.title_ = new createjs.Text('HISTORY', '30px ' + CONST.FONT.ALL, '#fff');
	this.title_.set(HistoryUI.settings.title);
	this.addChild(this.title_);
	this.back_ = new EsButton('Back', {font: '30px ' + CONST.FONT.ALL,
      fontColor: '#fff',
      borderColor: 'rgba(255,255,255,1)',
      bgColor: '#FBD00B',
      border: 2,
      enableShadow: false,
      width: CONST.HISTORY_UI.BACK_WIDTH,
      height: CONST.HISTORY_UI.BACK_HEIGHT});
     this.back_.set({x:CONST.BG.WIDTH / 2 - CONST.HISTORY_UI.BACK_WIDTH / 2,y:CONST.HISTORY_UI.PANEL_HEIGHT - 20 - CONST.HISTORY_UI.BACK_HEIGHT + CONST.HISTORY_UI.PANEL_FILL_HEIGHT});
     this.addChild(this.back_);

}

HistoryUI.prototype.show = function(){
	this.y = this.y - CONST.HISTORY_UI.PANEL_HEIGHT - CONST.HISTORY_UI.PANEL_FILL_HEIGHT;
	this.visible = true;
	EaselAnimationHelper.moveTo(this, this.x, 0 - CONST.HISTORY_UI.PANEL_FILL_HEIGHT, CONST.TIME.HISTORY_SWITCH, createjs.Ease.elasticOut);
};

HistoryUI.prototype.hide = function(){
	EaselAnimationHelper.moveTo(this, this.x, this.y - CONST.HISTORY_UI.PANEL_HEIGHT - CONST.HISTORY_UI.PANEL_FILL_HEIGHT, 400, createjs.Ease.backIn).call(Delegate.create(this,function(){
		this.visible = false;
	}));	
};

HistoryUI.settings = {
	title: {x: CONST.BG.WIDTH / 2,y: CONST.BAR.HEIGHT / 2 + CONST.HISTORY_UI.PANEL_FILL_HEIGHT, textAlign: 'center', textBaseline: 'middle'}
};

HistoryUI.prototype.initHistoryList = function(historyList){
	var count = historyList.length;
	this.historyListPanel_.removeAllChildren();
	BOK.each(this.historyBars_, function(item){
		item.removeAllEventListeners();
	});
	this.historyBars_ = [];
	var i =0;
    BOK.each(historyList, function(history){
        var historyBar = this.generateBar_(history);
		historyBar.set({x: (CONST.BG.WIDTH - CONST.BAR.WIDTH) / 2, y: CONST.BAR.HEIGHT + CONST.HISTORY_UI.PANEL_FILL_HEIGHT + CONST.TOPIC_BAR.HEIGHT * i + CONST.TOPIC_UI.TOPIC_BAR_SPACE * i});
        this.historyListPanel_.addChild(historyBar);
        this.historyBars_.push(historyBar);
        EaselAnimationHelper.standOut(historyBar.getChildAt(0), 3, 600, i*200);
        i ++;
    }, this);
}

/**
 * generate history bar
 * @param {Object} history // the history object from back , data format: {
 *      topic: {string} // the lower string
 *      time: {number} // the history info create time
 *      score:{number} // my score
 *      oppScore: {number}// the opponent score
 *      opp:{Object} // the opponent  info , data format:
 *      {
 *          id: {string}
 *          avatar: {string},
 *          name: {string},
 *
 *      }
 * }
 * @returns {createjs.Container}
 * @private
 */
HistoryUI.prototype.generateBar_ = function (history) {
    var width = CONST.BAR.WIDTH;
    var height = CONST.BAR.HEIGHT;
    var radius = CONST.HISTORY_UI.AVATAR_RADIUS;
    var iconSize = CONST.HISTORY_UI.ICON_SIZE;
    var space = CONST.HISTORY_UI.SPACE;
    var resultFlag = (history.score - history.oppScore);
    var resultText = 'You ' + (resultFlag > 0 ? 'win' : (resultFlag == 0 ? 'tie' : 'lost')) + ' to ' + history.opp.name + ' ' + moment(history.time).fromNow();


    var bar = new createjs.Container();
    var bg = new createjs.Shape(new createjs.Graphics().beginFill('#fff').drawRoundRect(0, 0, width, height, 4).endFill());
    bar.addChild(bg);

    var topicText = history.topic;
    topicText = topicText.replace(/\b\w+\b/g, function(word){return word.substring(0,1).toUpperCase()+word.substring(1);});

    var topic = new createjs.Text(topicText, '30px ' + CONST.FONT.ALL, '#000');
    var result = new createjs.Text(resultText, '18px ' + CONST.FONT.ALL, '#FF5454');
    var icon = new SosaIcon(TopicsUI.icon[history.topic], iconSize, {border: 0, bgColor: 'rgba(255,255,255,0)'});

    var avatar = new quiz.features.game.v.component.Avatar(radius, '#9873E6', history.opp.avatar, 0.25);

    topic.set({x: iconSize + space * 2, y: space * 2, textAlign: 'left', textBaseline: 'middle'});
    result.set({x: iconSize + space * 2, y: height - space * 2, textAlign: 'left', textBaseline: 'middle', maxWidth: width - (space * 4 + iconSize + radius* 2)});
    icon.set({x: space, y: space / 2});
    avatar.set({x: width - space - radius, y: height / 2});

    bar.addChild(icon, topic, avatar, result);
    return bar;
};
