/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-10-15
 * Time: 上午1:51
 * Write the description in this section.
 */
goog.provide('miniiw.features.factoryminigame.view.UnstableGrid');
goog.require("bok.features.basicmatch3.v.components.GridBoard");

miniiw.features.factoryminigame.view.UnstableGrid = function() {
    bok.features.basicmatch3.v.components.GridBoard.call(this);

    this.unstable_ = false;

};
BOK.inherits(miniiw.features.factoryminigame.view.UnstableGrid, bok.features.basicmatch3.v.components.GridBoard);


/**
 * @override
 * */
miniiw.features.factoryminigame.view.UnstableGrid.prototype.init_ = function()
{
    miniiw.features.factoryminigame.view.UnstableGrid.superClass_.init_.call(this);
    //init grid
    var SETTINGS = CONST.GRID_SETTINGS.GRID_BOARD;
    var ICON_SETTINGS = CONST.GRID_SETTINGS.ICON;

    this.frame2_ = new createjs.Shape();
    this.frame2_.graphics.beginFill('rgba(200,20,20,1)').drawRect(
        -SETTINGS.FRAME_WIDTH, -SETTINGS.FRAME_WIDTH,
        SETTINGS.FRAME_WIDTH2*2 + ICON_SETTINGS.WIDTH*SETTINGS.WIDTH,
        SETTINGS.FRAME_WIDTH2);
    this.frame2_.graphics.beginFill('rgba(200,20,20,1)').drawRect(
        -SETTINGS.FRAME_WIDTH, SETTINGS.FRAME_WIDTH2 + ICON_SETTINGS.HEIGHT*SETTINGS.HEIGHT - SETTINGS.FRAME_WIDTH,
        SETTINGS.FRAME_WIDTH2*2 + ICON_SETTINGS.WIDTH*SETTINGS.WIDTH,
        SETTINGS.FRAME_WIDTH2);
    this.frame2_.graphics.beginFill('rgba(200,20,20,1)').drawRect(
        -SETTINGS.FRAME_WIDTH, -SETTINGS.FRAME_WIDTH,
        SETTINGS.FRAME_WIDTH2,
        SETTINGS.FRAME_WIDTH2*2 + ICON_SETTINGS.HEIGHT*SETTINGS.HEIGHT);
    this.frame2_.graphics.beginFill('rgba(200,20,20,1)').drawRect(
        SETTINGS.FRAME_WIDTH2 + ICON_SETTINGS.WIDTH*SETTINGS.WIDTH - SETTINGS.FRAME_WIDTH, -SETTINGS.FRAME_WIDTH,
        SETTINGS.FRAME_WIDTH2,
        SETTINGS.FRAME_WIDTH2*2 + ICON_SETTINGS.HEIGHT*SETTINGS.HEIGHT);
    this.frame2_.visible = false;

    this.root_.addChild(this.frame2_);
};


/**
 * */
miniiw.features.factoryminigame.view.UnstableGrid.prototype.unstabilize = function(){
    if(this.unstable_)
        return;

    this.unstable_ = true;
    this.frame2_.visible = true;
    this.frame_.visible = false;
};
/**
 * */
miniiw.features.factoryminigame.view.UnstableGrid.prototype.stabilize = function(){
    this.unstable_ = false;
    this.frame_.visible = true;
    this.frame2_.visible = false;
};

/**
 * */
miniiw.features.factoryminigame.view.UnstableGrid.prototype.clearRow = function(row, advClear)
{
    if(this.locked) {
        this.unlock();
    }

    if(advClear)
        this.unstabilize();

    var totalClear = 0;
    BOK.loop(bok.features.basicmatch3.v.components.GridBoard.SETTINGS.WIDTH, Delegate.create(this, function(i){
        this.removeOnPos_(i, row) && totalClear++;
        if(advClear) {
            setTimeout(Delegate.create(this, function(i){
                this.genFlair_(i, row-1) && totalClear++;
                this.genFlair_(i, row+1) && totalClear++;
            }, i), i*190);
        }
    }), this);
    setTimeout(Delegate.create(this, function(){
        var e = new createjs.Event('massClearScore');
        e.clearCount = totalClear;
        this.dispatchEvent(e);
    }), bok.features.basicmatch3.v.components.GridBoard.SETTINGS.MOVE_DURATION + 800);
    setTimeout(Delegate.create(this, this.onReactFinish_), bok.features.basicmatch3.v.components.GridBoard.SETTINGS.MOVE_DURATION + 800);
};
/**
 * */
miniiw.features.factoryminigame.view.UnstableGrid.prototype.clearColumn = function(col, advClear)
{
    if(this.locked) {
        this.unlock();
    }

    if(advClear)
        this.unstabilize();

    var totalClear = 0;
    BOK.loop(bok.features.basicmatch3.v.components.GridBoard.SETTINGS.HEIGHT, Delegate.create(this, function(i){
        this.removeOnPos_(col, i) && totalClear++;
        if(advClear) {
            setTimeout(Delegate.create(this, function(i){
                this.genFlair_(col-1, i) && totalClear++;
                this.genFlair_(col+1, i) && totalClear++;
            }, i), i*190);
        }
    }), this);
    setTimeout(Delegate.create(this, function(){
        var e = new createjs.Event('massClearScore');
        e.clearCount = totalClear;
        this.dispatchEvent(e);
    }), bok.features.basicmatch3.v.components.GridBoard.SETTINGS.MOVE_DURATION + 800);
    setTimeout(Delegate.create(this, this.onReactFinish_), bok.features.basicmatch3.v.components.GridBoard.SETTINGS.MOVE_DURATION + 800);
};


/**
 * */
miniiw.features.factoryminigame.view.UnstableGrid.prototype.clearColor = function(type){
    if(this.locked) {
        return;
    }

    var totalClear = 0;
    BOK.loop(bok.features.basicmatch3.v.components.GridBoard.SETTINGS.WIDTH, Delegate.create(this, function(i){
        BOK.loop(bok.features.basicmatch3.v.components.GridBoard.SETTINGS.HEIGHT, Delegate.create(this, function(j){
            setTimeout(Delegate.create(this, function(i){
                if(this.icons_[i][j] && this.icons_[i][j].type == type){
                    if(this.removeOnPos_(i, j)){
                        this.flairExplode_(i, j);
                        totalClear++;
                    }
                }
            }, i), BOK.randN(600));
        }), this);
    }), this);
    setTimeout(Delegate.create(this, function(){
        var e = new createjs.Event('massClearScore');
        e.clearCount = totalClear;
        this.dispatchEvent(e);
    }), 600);
};


