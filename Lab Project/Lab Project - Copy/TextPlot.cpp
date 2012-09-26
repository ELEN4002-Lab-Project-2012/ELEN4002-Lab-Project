/**
 * @file TextPlot.cpp
 *
 * A simple console-based data plotter.
 *
 * This file is part of the Aquila DSP library.
 * Aquila is free software, licensed under the MIT/X11 License. A copy of
 * the license is provided with the library in the LICENSE file.
 *
 * @package Aquila
 * @version 3.0.0-dev
 * @author Zbigniew Siciarz
 * @date 2007-2011
 * @license http://www.opensource.org/licenses/mit-license.php MIT
 * @since 3.0.0
 */

#include "TextPlot.h"
#include <boost/shared_array.hpp>
#include <fstream>
#include <iostream>
using namespace std;

namespace Aquila
{
    /**
     * Creates the plot object.
     *
     * @param title plot title (optional, default is "Data plot")
     * @param out where to output the plot data (default is std::cout)
     */
    TextPlot::TextPlot(const std::string &title, std::ostream &out):
        m_title(title), m_out(out), m_width(64), m_height(16)
    {
    }

    /**
     * "Draws" the plot to the output stream.
     *
     * @param plot internal plot data
     */
    void TextPlot::drawPlotMatrix(const PlotMatrixType &plot)
    {
        const std::size_t length = plot.size();

        m_out << "\n" << m_title << "\n";
        // output the plot data, flushing only at the end
        for (unsigned int y = 0; y < m_height; ++y)
        {
            for (unsigned int x = 0; x < length; ++x)
            {
                m_out << plot[x][y];
            }
            m_out << "\n";
        }
        m_out.flush();
    }

    /**
     * A shorthand for plotting only the first half of magnitude spectrum.
     *
     * @param spectrum an array of complex numbers
     * @param length size of the spectrum (total, not half!)
     */
    void TextPlot::plotSpectrum(Aquila::ComplexType spectrum[], std::size_t length)
    {
        boost::shared_array<double> absSpectrum(new double[length/2]);
        for (std::size_t i = 0; i < length/2; ++i)
        {
            absSpectrum[i] = std::abs(spectrum[i]);
        }
        plot(absSpectrum.get(), length/2);

    }
}
