/**
 * Created by Envee.
 *
 * Date: 14-10-10
 * Time: 下午4:46
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("noone.features.game.v.component.MainMenu");
goog.require("noone.features.game.v.component.Button");
goog.require("org.createjs.easeljs.EaselJS");

noone.features.game.v.component.MainMenu = MainMenu;
BOK.inherits(MainMenu, createjs.Container);
function MainMenu(){
    createjs.Container.call(this);

    this.menuStart_ =new noone.features.game.v.component.Button().set({x: 260, y: 150});
    this.menuHelp_ =new noone.features.game.v.component.Button('游戏帮助').set({x: 260, y: 210});
    this.addChild(new createjs.Shape(new createjs.Graphics().beginFill("#fff").drawRect(0, 0, 640, 480)));
    this.menuStart_.text.text = "开始游戏";
    this.addChild(this.menuStart_, this.menuHelp_);
}

MainMenu.prototype.hide = function(){
    this.visible = false;
};

MainMenu.prototype.show = function(){
    this.visible = true;
};