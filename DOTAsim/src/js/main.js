/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 04/10/13
 * Time: 16:18
 * To change this template use File | Settings | File Templates.
 */

goog.provide("root.main");
goog.require("bok.apps.preloader.CanvasPreloaderApp");
goog.require("bok.apps.splashscreen.SplashScreenApp");
goog.require("dotasim.DOTASimApp");
goog.requireAll('dotasim.assets.data.*');

var preloaderApp, game, rootStage;
window.addEventListener('load', start);

//DEBUG options
BOK.enableTrace = false;
LOCAL_DEBUG = {
    SHOW_GRID: false
};

function start()
{
    rootStage = new createjs.Stage(document.getElementById('canvas'));
    createjs.Ticker.setFPS(40);
    createjs.Ticker.addEventListener("tick", Delegate.create(rootStage, rootStage.update));

    var splashScreen = new SplashScreenApp(rootStage);
    splashScreen.addEventListener(Event.COMPLETE, loadingStart);

    splashScreen.start();
}

imgContainer = {};
function loadingStart()
{
    preloaderApp = new CanvasPreloaderApp([
        'assets/img/iw1.png', 'assets/img/iw1.png', 'assets/img/iw1.png',
        'assets/img/iw1.png', 'assets/img/iw1.png', 'assets/img/iw2.png'], imgContainer, rootStage);
    preloaderApp.preloadFonts(['Sosa']);
    preloaderApp.addEventListener(Event.COMPLETE, assetsLoaded);

    preloaderApp.start();
}

function assetsLoaded()
{
    game = new DOTASimApp(rootStage);
    game.start();
}
