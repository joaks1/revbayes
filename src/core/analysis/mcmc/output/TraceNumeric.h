//
//  TraceNumeric.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 4/4/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#ifndef TraceNumeric_H
#define TraceNumeric_H

#include "Trace.h"

#include <vector>

namespace RevBayesCore {

    class TraceNumeric : public Trace<double> {
    
    public:
        TraceNumeric();
        virtual ~TraceNumeric(){};

        virtual TraceNumeric*   clone(void) const;                              //!< Clone object

        double                  getMean() const;                                //!< compute the mean for the trace
        double                  getESS() const;                                 //!< compute the effective sample size
        double                  getSEM() const;                                 //!< compute the standard error of the mean

        double                  getMean(long begin, long end) const;            //!< compute the mean for the trace with begin and end indices of the values
        double                  getESS(long begin, long end) const;             //!< compute the effective sample size with begin and end indices of the values
        double                  getSEM(long begin, long end) const;             //!< compute the effective sample size with begin and end indices of the values

        void                    computeStatistics(void);

        //int                     hasConverged() const                            { return converged; }
        int                     hasPassedGewekeTest() const                     { return passedGewekeTest; }
        int                     hasPassedStationarityTest() const               { return passedStationarityTest; }
        //int                     hasPassedEssThreshold() const                   { return passedEssThreshold; }
        //int                     hasPassedSemThreshold() const                   { return passedSemThreshold; }
        //int                     hasPassedIidBetweenChainsStatistic() const      { return passedIidBetweenChainsStatistic; }
        //int                     hasPassedGelmanRubinTest() const                { return passedGelmanRubinTest; }

    protected:

        void                    update() const;                                 //!< compute the correlation statistics (act,ess,sem,...)
        void                    update(long begin, long end) const;             //!< compute the correlation statistics (act,ess,sem,...)

        // variable holding the data
        mutable double          ess;                                            //!< effective sample size
        mutable double          mean;                                           //!< mean of trace
        mutable double          sem;                                            //!< standard error of mean

        mutable long            begin;
        mutable long            end;

        // variable holding the data
        mutable double          essw;                                            //!< effective sample size
        mutable double          meanw;                                           //!< mean of trace
        mutable double          semw;                                            //!< standard error of mean

        //int                     converged;                                      //!< Whether this parameter in itself has converged.
        int                     passedStationarityTest;                         //!< Whether this parameter passed the stationarity test.
        int                     passedGewekeTest;                               //!< Whether this parameter passed the Geweke statistic.
        //int                     passedEssThreshold;                             //!< Whether this parameter passed the threshold for the ESS.
        //int                     passedSemThreshold;                             //!< Whether this parameter passed the threshold for the SEM.
        //int                     passedIidBetweenChainsStatistic;                //!< Whether this parameter passed the iid test of chains.
        //int                     passedGelmanRubinTest;                          //!< Whether this parameter passed the Gelman-Rubin statistic.

        mutable bool            stats_dirty;
        mutable bool            statsw_dirty;
    
    };

}

#endif



