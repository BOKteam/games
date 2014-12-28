/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 上午11:18
 * Write the description in this section.
 */
goog.provide("app.BirdApp");
goog.require("bok.framework.App");

goog.require("app.features.game.MainGameFeature");

/**
 * @param{createjs.Container} stage
 * */
BOK.inherits(BirdApp, App);
function BirdApp(stage) {
    App.call(this);

    this.addFeature(new app.features.game.MainGameFeature(stage));
}