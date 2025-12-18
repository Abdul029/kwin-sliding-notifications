#include <effect/effect.h>
#include <effect/effectwindow.h>
#include <effect/animationeffect.h>
#include <effect/effecthandler.h>
#include <KPluginFactory>
#include <QEasingCurve>
#include <QElapsedTimer>
#include <QMap>

namespace KWin {

class SlidingEffect : public AnimationEffect {
    Q_OBJECT
public:
    explicit SlidingEffect(QObject *parent = nullptr, const QVariantList &args = QVariantList())
        : AnimationEffect() {
        Q_UNUSED(parent);
        Q_UNUSED(args);

        connect(effects, &EffectsHandler::windowAdded, this, &SlidingEffect::onWindowAdded);
        connect(effects, &EffectsHandler::windowClosed, this, &SlidingEffect::onWindowClosed);
        connect(effects, &EffectsHandler::windowDeleted, this, &SlidingEffect::onWindowDeleted);
    }

    void onWindowAdded(EffectWindow *w) {
        if (w->isNotification()) {
            animate(w, AnimationEffect::Translation, 0, 350, FPx2(0.0), 
                    QEasingCurve::OutBack, 0, FPx2(400.0));
            animate(w, AnimationEffect::Opacity, 0, 250, FPx2(1.0), 
                    QEasingCurve::Linear, 0, FPx2(0.0));
        }
    }

    void onWindowClosed(EffectWindow *w) {
        if (w->isNotification()) {
            QElapsedTimer *timer = new QElapsedTimer();
            timer->start();
            m_timers[w] = timer;

            // Trigger long keep-alive to ensure window isn't destroyed mid-animation
            animate(w, AnimationEffect::Opacity, 0, 500, FPx2(0.0), 
                    QEasingCurve::Linear, 0, FPx2(1.0), false, true);
        }
    }

    void onWindowDeleted(EffectWindow *w) {
        if (m_timers.contains(w)) {
            delete m_timers.take(w);
        }
    }

    void drawWindow(const RenderTarget &renderTarget, const RenderViewport &viewport, EffectWindow *w, int mask, const QRegion &region, WindowPaintData &data) override {
        if (w->isNotification()) {
            data.setYTranslation(0);
            data.setZTranslation(0);
            
            if (m_timers.contains(w)) {
                qint64 elapsed = m_timers[w]->elapsed();
                
                // Cut the render feed at 200ms to hide the compositor's cleanup stutter
                if (elapsed > 200) {
                    return; 
                }

                float progress = static_cast<float>(elapsed) / 200.0f;
                data.setXTranslation(500.0f * progress);
                data.setOpacity(1.0f - progress);
            }
        }
        AnimationEffect::drawWindow(renderTarget, viewport, w, mask, region, data);
    }

private:
    QMap<EffectWindow*, QElapsedTimer*> m_timers;
};

} // namespace KWin

KWIN_EFFECT_FACTORY_SUPPORTED(KWin::SlidingEffect, "kwin_slidingnotifs.json", return true;)
#include "SlidingEffect.moc"
