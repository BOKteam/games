/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.RematchTopicsUI");
goog.require("org.createjs.easeljs.EaselJS");
goog.require("bok.easelui.SosaIcon");
goog.require("bok.util.EaselAnimationHelper");
goog.require("quiz.features.game.v.component.TopicsUI");
goog.require('quiz.AssetsList');

quiz.features.game.v.component.RematchTopicsUI = RematchTopicsUI;
BOK.inherits(RematchTopicsUI, quiz.features.game.v.component.TopicsUI);

function RematchTopicsUI (){
	quiz.features.game.v.component.TopicsUI.call(this);
}