import { useRecentWeatherData } from "@/hooks/useRecentWeatherData.ts";
import MetricChart from "@/components/MetricChart.tsx";
import { useMemo, useState } from "react";
import { ChartControls } from "@/components/ChartControls.tsx";
import { exponentialSmoothing } from "@/utils/smoothing.ts";
import type { WeatherReading } from "@/services/types.ts";

const WeatherChartsWidget = () => {
    const [viewMode, setViewMode] = useState<'raw' | 'smoothed'>('smoothed');
    const [smoothingIntensity, setSmoothingIntensity] = useState(0.8);
    const { data, isLoading, error } = useRecentWeatherData("24");

    const formattedData = useMemo(() => {
        if (!data) return [];
        return data.map((reading: WeatherReading) => ({
            ...reading,
            timestamp: new Date(reading.timestamp).toLocaleTimeString('en-GB', { hour: '2-digit', minute: '2-digit' }),
        }));
    }, [data]);

    const smoothedData = useMemo(() => {
        if (viewMode === 'raw' || !formattedData) return [];
        const alpha = 1 - smoothingIntensity;

        return exponentialSmoothing(formattedData, ['temperature', 'humidity', 'pressure'], alpha);
    }, [formattedData, smoothingIntensity, viewMode]);

    if (error) return <div>Error: {error}</div>;

    const chartData = viewMode === 'smoothed' ? smoothedData : formattedData;

    return (
        <div className="mt-4">
            <h2 className="text-2xl font-bold mb-4">Historical Data</h2>

            <ChartControls
                viewMode={viewMode}
                onViewModeChange={setViewMode}
                smoothingIntensity={smoothingIntensity}
                onSmoothingIntensityChange={setSmoothingIntensity}
            />


            <div className={"flex flex-col gap-4"}>
                <MetricChart metric="temperature" data={chartData} isLoading={isLoading} />
                <MetricChart metric="humidity" data={chartData} isLoading={isLoading} />
                <MetricChart metric="pressure" data={chartData} isLoading={isLoading} />
            </div>
        </div>
    );
};

export default WeatherChartsWidget;