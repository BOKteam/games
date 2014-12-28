/**
 * Created by Envee.
 *
 * Date: 14-10-11
 * Time: 下午2:00
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("noone.features.game.v.component.EndPanel");
goog.require("noone.features.game.v.component.Button");
goog.require("org.createjs.easeljs.EaselJS");

noone.features.game.v.component.EndPanel = EndPanel;
BOK.inherits(EndPanel, createjs.Container);

function EndPanel(){
    createjs.Container.call(this);

    this.content_ = new createjs.Shape(new createjs.Graphics().beginStroke('#D77D56').beginFill('#fff').drawRect(0,0,300,280));
    this.set({x :150, y: 60});
    this.titleText_ = new createjs.Text('Game Over',"bold 20px dimboregular", "#000").set({x: 10, y: 10});
    this.thisScoreText_ = new createjs.Text('本轮分数',"bold 10px dimboregular", "#000").set({x: 130, y: 50});
    this.thisScore_ = new createjs.Text('',"28px dimboregular", "#000").set({x: 130, y: 65});
    this.histScoreText_ = new createjs.Text('历史最高记录',"bold 15px dimboregular", "#000").set({x: 110, y: 120});
    this.histScore_ = new createjs.Text(localStorage.highScore,"10px dimboregular", "#000").set({x:140, y:140});

    this.submitBtn_ = new noone.features.game.v.component.Button("提交积分").set({x:20, y:200});
    this.backBtn_ = new noone.features.game.v.component.Button("返回菜单").set({x: 160, y: 200});

    this.addChild(this.content_, this.titleText_, this.thisScoreText_, this.thisScore_, this.histScoreText_, this.histScore_, this.submitBtn_, this.backBtn_);

}

EndPanel.prototype.setThisScore = function(score){
    this.thisScore_.text = score;
};
