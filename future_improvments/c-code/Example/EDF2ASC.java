package com.srresearch.edf2asc;

/**
 * @author suganthan
 *
 *
 */
import javax.swing.*;

import com.srresearch.edfapi.*;
public class EDF2ASC
{
	public static void main(String s[]) throws Exception
	{
		long start = System.currentTimeMillis();
		DefaultOptionAdapter options = new DefaultOptionAdapter()
		{
			public boolean outputSampleVelocity()
			{
				return true;
			}
		};

		JWindow window = new JWindow();
		window.setLocation(window.getToolkit().getScreenSize().width/2,
					 	   window.getToolkit().getScreenSize().height/2);
		window.setVisible(true);
		ProgressMonitor  progress = new ProgressMonitor(window,"Loading " , s[0],0,100);
		EDF edf = new EDF(options, s[0],2,true, true,0);
		edf.setTrialIdentifiers("TRIALID","TRIAL_RESULT");
		edf.jumpToTrial(0);





		System.out.print(edf.getPreambleText());
		System.out.println("**\n");

		boolean done = false;
		start = System.currentTimeMillis();
		int fixcount =0;
		int elementcount = edf.getItemCount();
		int curelem = 0;
		while(!done  && !progress.isCanceled())
		{
			int type = edf.getNextData();



			if(type != EDF.NO_PENDING_ITEMS)
			{
				curelem++;
				EDFElement data = edf.getFloatData();

				if(type != EDF.RECORDING_INFO)
					data = edf.getFloatData();
				System.out.print(data);
				if(type == EDF.ENDFIX)
					fixcount ++;
				int perc = (curelem*100)/elementcount;
				progress.setProgress(perc);
				if(type == EDF.RECORDING_INFO)
				{
					System.err.println("fixcount "+ fixcount);
					fixcount =0;
				}

			}
			else
			{

				done = true;
			}
		}
		System.err.println("frame number " + edf.findFrameForEDFTime(-1930800));
		System.err.println("writing  time "+ (double)((System.currentTimeMillis() - start))/1000.0);
		window.setVisible(false);

		System.exit(0);
	}
}
