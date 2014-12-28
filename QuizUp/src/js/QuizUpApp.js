/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午8:56
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("quiz.QuizUpApp");
goog.require("bok.framework.App");

goog.require("quiz.features.game.MainGameFeature");

/**
 * @param{createjs.Container} stage
 * */
BOK.inherits(QuizUpApp, App);
function QuizUpApp(stage) {
    App.call(this);

    this.addFeature(new quiz.features.game.MainGameFeature(stage));
}