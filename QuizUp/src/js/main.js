/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午8:55
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("root.main");

goog.require("bok.easelui.Stretcher");
goog.require("bok.apps.preloader.CanvasPreloaderApp");
goog.require("bok.apps.preloader.components.BlueCanvasSkin");
goog.require("quiz.QuizUpApp");
goog.require("quiz.AssetsList");

var preloaderApp, game, rootStretcher, imgContainer = {};
window.addEventListener('load', start);

function start() {
    rootStretcher = new Stretcher(document.getElementById('canvas'),540, 960);
    rootStretcher.hideFPS();
    var loadingBG = new createjs.Bitmap('assets/img/ingame_en.png');
    var loadCanvasSkin = new BlueCanvasSkin(loadingBG, 'rgba(100,200,255,0.8)');
    preloaderApp = new CanvasPreloaderApp(imgList, imgContainer, rootStretcher, loadCanvasSkin);
    loadingBG.set({x:250, y:-50});
    loadCanvasSkin.set({x:-120, y:150});
    EaselAnimationHelper.bounceTo(loadingBG, 250, 150, 1000);
    preloaderApp.preloadFonts(['ArimoBold']);
    preloaderApp.setLoadInterval(20);
    preloaderApp.addEventListener(Event.COMPLETE, assetsLoaded);

    preloaderApp.start();
}

function assetsLoaded() {
    game = new QuizUpApp(rootStretcher);
    game.start();
}

