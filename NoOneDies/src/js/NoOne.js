/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 上午11:18
 * Write the description in this section.
 */
goog.provide("noone.NoOneApp");
goog.require("bok.framework.App");

goog.require("noone.features.game.MainGameFeature");

/**
 * @param{createjs.Container} stage
 * */
BOK.inherits(NoOneApp, App);
function NoOneApp(stage) {
    App.call(this);

    this.addFeature(new noone.features.game.MainGameFeature(stage));
}