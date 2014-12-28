/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午8:56
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("tzfe.TzfeApp");
goog.require("bok.framework.App");

goog.require("tzfe.features.game.MainGameFeature");

/**
 * @param{createjs.Container} stage
 * */
BOK.inherits(TzfeApp, App);
function TzfeApp(stage) {
    App.call(this);

    this.addFeature(new tzfe.features.game.MainGameFeature(stage));
}