/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 下午4:34
 * Write the description in this section.
 */
goog.provide("app.features.game.v.component.Pipe");
goog.require("org.createjs.easeljs.EaselJS");


app.features.game.v.component.Pipe = Pipe;
BOK.inherits(Pipe, createjs.Container);
function Pipe(){
    createjs.Container.call(this);
    
    var offsetW = 6,
        pipeHmin = 50,
        offsetYmax = CONST.BG.HEIGHT  - CONST.PIPE.APART - pipeHmin,
        posX = CONST.CANVAS.WIDTH;
    this.posY = Math.random()*(offsetYmax - pipeHmin) + pipeHmin;

    var pipe1 = new createjs.Bitmap(imgContainer['assets/img/pipe.png']);
    var pipe2 = new createjs.Bitmap(imgContainer['assets/img/pipe.png']);
    pipe1.set({ x:posX, y: -CONST.PIPE.HEIGHT + this.posY });
    pipe2.setTransform(posX + CONST.PIPE.WIDTH + offsetW, CONST.PIPE.HEIGHT + this.posY + CONST.PIPE.APART, 1, 1, 180);

    this.addChild(pipe1, pipe2);
}

Pipe.prototype.updateAndCheckRemove = function() {
    this.x -= CONST.GAME_PLAY.SCROLL_SPEED;
};

