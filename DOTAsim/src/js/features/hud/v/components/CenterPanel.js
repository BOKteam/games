/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 10/03/14
 * Time: 15:33
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.features.hud.v.components.CenterPanel");
goog.require("bok.easelui.EsPanel");

goog.require("dotasim.features.hud.v.components.HeroIcon");

dotasim.features.hud.v.components.CenterPanel = function()
{
    var settings = dotasim.GameSettings.GameHud.CenterPanel;
    EsPanel.call(this, settings.WIDTH, settings.HEIGHT, {border:settings.BORDER});
    this.set({
        x:settings.X,
        y:settings.Y,
        scaleX: 0,
        scaleY: 0
    });

    this.heroIcons_ = {};
    this.playerData_ = {};

    this.initPlayerGoldDisplay_(settings);

    createjs.Ticker.addEventListener('tick', Delegate.create(this, this.onTickUpdate_));
};
BOK.inherits(dotasim.features.hud.v.components.CenterPanel, EsPanel);

/**
 * @override
 * */
dotasim.features.hud.v.components.CenterPanel.prototype.hide = function()
{
   BOK.each(this.heroIcons_, function(hero){
        hero.unselect();
    });

    dotasim.features.hud.v.components.CenterPanel.superClass_.hide.call(this);
};

/**
 * @param {dotasim.features.hud.v.components.HeroIcon} heroIcon
 * */
dotasim.features.hud.v.components.CenterPanel.prototype.releaseHeroIcon = function(heroIcon)
{
    heroIcon.removeEventListener(Event.SELECTED, heroIcon.eventListenerOnHeroSelected);
    heroIcon.removeEventListener(Event.UNSELECTED, heroIcon.eventListenerOnHeroUnselected);
    delete this.heroIcons_[heroIcon.id];

    this.positionIcons_();
};

/**
 * @param {dotasim.features.hud.v.components.HeroIcon} heroIcon
 * */
dotasim.features.hud.v.components.CenterPanel.prototype.regainHeroIcon = function(heroIcon)
{
    this.addHero_(heroIcon);
};

/**
 * @param {Object} data
 * */
dotasim.features.hud.v.components.CenterPanel.prototype.setPlayerData = function(data)
{
    this.playerData_ = data;
};

/**
 * @param {Object} heroData
 * */
dotasim.features.hud.v.components.CenterPanel.prototype.addHero = function(heroData)
{
    if(!this.heroIcons_[heroData.id])
    {
        var hero = new dotasim.features.hud.v.components.HeroIcon(heroData);
        hero.eventListenerOnHeroSelected = Delegate.create(this, this.onHeroSelected_);
        hero.eventListenerOnHeroUnselected = Delegate.create(this, this.onHeroUnselected_);
        this.addHero_(hero);
    }
};

/**
 * @private
 * */
dotasim.features.hud.v.components.CenterPanel.prototype.positionIcons_ = function()
{
    var count = 0;
    for(var id in this.heroIcons_)
    {
        var newPos = {x: 35 * count++, y:0};

        if(this.heroIcons_[id].x != newPos.x || this.heroIcons_[id].y != newPos.y)
       {
           createjs.Tween.get(this.heroIcons_[id]).to(newPos, 500, createjs.Ease.bounceOut);
       }
    }
};
/**
 * @param {dotasim.features.hud.v.components.HeroIcon} heroIcon
 * */
dotasim.features.hud.v.components.CenterPanel.prototype.addHero_ = function(heroIcon)
{
    heroIcon.addEventListener(Event.SELECTED, heroIcon.eventListenerOnHeroSelected);
    heroIcon.addEventListener(Event.UNSELECTED, heroIcon.eventListenerOnHeroUnselected);
    this.addChild(heroIcon);
    heroIcon.x = heroIcon.y = -50;
    EaselAnimationHelper.fadeIn(heroIcon, 500);

    this.heroIcons_[heroIcon.id] = heroIcon;

    this.positionIcons_();
};


dotasim.features.hud.v.components.CenterPanel.prototype.onHeroUnselected_ = function(e)
{
    this.dispatchEvent({type:Event.UNSELECTED});
};

dotasim.features.hud.v.components.CenterPanel.prototype.onHeroSelected_ = function(e)
{
    var hero = e.target;
    BOK.each(this.heroIcons_, function(icon){
        if(icon.id != hero.id)
            icon.unselect();
    });

    this.dispatchEvent({type:Event.SELECTED, hero:hero});
};

dotasim.features.hud.v.components.CenterPanel.prototype.onTickUpdate_ = function()
{
    if(this.recordedPlayerGold_ != this.playerData_.gold)
    {
        this.recordedPlayerGold_ = this.playerData_.gold;
        this.playerGoldText_.text = this.recordedPlayerGold_;
    }
};

dotasim.features.hud.v.components.CenterPanel.prototype.initPlayerGoldDisplay_ = function(parentSettings)
{
    var settings = parentSettings.PlayerGoldDisplay;
    this.playerGoldDisplay_ = new EsPanel(settings.WIDTH, settings.HEIGHT,
        {useDefaultCloseButton:false, border:settings.BORDER, bgColor:'#FFF'});
    this.playerGoldDisplay_.x = 0;
    this.playerGoldDisplay_.y = -(settings.HEIGHT + settings.BORDER + parentSettings.BORDER);
    this.addChild(this.playerGoldDisplay_);

    var moneySymbol = new SosaIcon(SosaIcon.ICON.CREDIT_CARD, settings.SYMBOL.SIZE, {border:settings.SYMBOL.BORDER});
    this.playerGoldDisplay_.addChild(moneySymbol);

    this.playerGoldText_ = new createjs.Text('0', 'bold 20px Arial', 'rgb(200, 200, 10)');
    this.playerGoldText_.x = moneySymbol.width + settings.SYMBOL.BORDER;
    this.playerGoldDisplay_.addChild(this.playerGoldText_);
};