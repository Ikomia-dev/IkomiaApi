//-----------------------------------------------------------------
// This class shows how to extend QwtPlotItems. It displays a
// pie chart of user/total/idle cpu usage in percent.
//-----------------------------------------------------------------

#include <qwt_plot_item.h>

/** @cond INTERNAL */

/**
 * @brief The PieMarker class creates the pie chart
 */
class CPieMarker: public QwtPlotItem
{
    private:
        int height = 300;
        int width = 300;
        int margin = 10;
        int numPlots = 0;

    public:
        CPieMarker();
        CPieMarker(int numPlots, int size);

        virtual int rtti() const;

        virtual void draw(QPainter *p, const QwtScaleMap &, const QwtScaleMap &, const QRectF &rect) const;

        void setNumberOfPlots(int totalPlots);
};

/** @endcond */
