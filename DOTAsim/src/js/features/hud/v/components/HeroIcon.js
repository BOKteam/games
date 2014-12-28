/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 10/03/14
 * Time: 11:08
 * To change this template use File | Settings | File Templates.
 */

goog.provide("dotasim.features.hud.v.components.HeroIcon");
goog.require("bok.easelui.SosaIcon");

/**
 * @param {Object} heroData generic hero unit data. @see {dotasim.features.maingame.m.model.defs}
 * */
dotasim.features.hud.v.components.HeroIcon = function(heroData)
{
    createjs.Container.call(this);

    this.id = heroData.id;
    this.heroData_ = heroData;
    this.selected_ = false;
    this.selectable_ = true;

    this.init_();

    this.addEventListener('click', Delegate.create(this, this.onClicked_));
    createjs.Ticker.addEventListener('tick', Delegate.create(this, this.onTickUpdate_));
};
BOK.inherits(dotasim.features.hud.v.components.HeroIcon, createjs.Container);


dotasim.features.hud.v.components.HeroIcon.prototype.unselect = function()
{
    if(this.selected_)
    {
        this.selected_ = false;
        this.base_.rotation = 0;
        createjs.Tween.removeTweens(this.base_);
        this.dispatchEvent(Event.UNSELECTED);
    }
};

dotasim.features.hud.v.components.HeroIcon.prototype.deployed = function()
{
    this.unselect();
    this.selectable_ = false;
    this.retractButton_.visible = true;
    createjs.Tween.get(this.retractButton_).to({y:this.icon_.height}, 1000, createjs.Ease.bounceOut);
};

dotasim.features.hud.v.components.HeroIcon.prototype.init_ = function()
{
    this.retractButton_ = new SosaIcon(SosaIcon.ICON.POP, 10, {width: 35, bgColor:'rgb(240, 30, 30)'});
    this.retractButton_.y = 0;
    this.retractButton_.visible = false;
    this.retractButton_.addEventListener(Event.CLICK, Delegate.create(this, this.onRetractClicked_));
    this.addChild(this.retractButton_);

    this.base_ = new createjs.Container();
    this.base_.x = 17;
    this.base_.y = 17;
    this.addChild(this.base_);

    this.icon_ = new SosaIcon(SosaIcon.ICON.MAN, 35);
    this.icon_.regX = 17;
    this.icon_.regY = 17;
    this.base_.addChild(this.icon_);

    this.levelText_ = new  createjs.Text('*1', 'bold 10px Arial', '#000');
    this.levelText_.x = this.levelText_.y = -15;
    this.levelText_.heroLevel = 1;
    this.base_.addChild(this.levelText_);
};

dotasim.features.hud.v.components.HeroIcon.prototype.swing_ = function()
{
    if(this.selected_)
    {
        createjs.Tween.get(this.base_)
            .to({rotation:15}, 200)
            .to({rotation:-15}, 400)
            .to({rotation:0}, 200)
            .call(Delegate.create(this, this.swing_));
    }
};
dotasim.features.hud.v.components.HeroIcon.prototype.retractFinished_ = function()
{
    this.selectable_ = true;
    this.retractButton_.visible = false;
    this.dispatchEvent('retract_finished');
};
dotasim.features.hud.v.components.HeroIcon.prototype.onRetractClicked_ = function(e)
{
    e.stopPropagation();
    createjs.Tween.get(this.retractButton_)
        .to({y:this.icon_.height - this.retractButton_.height}, dotasim.GamePlayData.GENERAL.HERO_RETRACT_TIME)
        .call(Delegate.create(this, this.retractFinished_));
    this.dispatchEvent('retract_started');
};
dotasim.features.hud.v.components.HeroIcon.prototype.onTickUpdate_ = function()
{
    //update level
    if(this.heroData_.level != this.levelText_.level)
    {
        this.levelText_.level = this.heroData_.level;
        this.levelText_.text = '*' + this.levelText_.level;
    }

    //update reborn count-down
    if(this.heroData_.state == 'dead')
    {
        var totalRebornTime = dotasim.GamePlayData.GENERAL.HERO_REBORN_TIME * this.heroData_.level;
        this.icon_.alpha = (totalRebornTime - this.heroData_.rebornTime) / totalRebornTime;
    }
};
dotasim.features.hud.v.components.HeroIcon.prototype.onClicked_ = function()
{
    if(this.selectable_)
    {
        if(!this.selected_)
        {
            this.selected_ = true;
            this.swing_();
            this.dispatchEvent(Event.SELECTED);
        }
        else
        {
            this.unselect();
        }
    }
};
