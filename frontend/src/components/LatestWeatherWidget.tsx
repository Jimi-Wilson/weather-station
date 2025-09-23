import {useLatestWeatherData} from "@/hooks/useLatestWeatherData.ts";
import MetricCard from "@/components/MetricCard.tsx";
import {Thermometer, Gauge, Droplets} from "lucide-react";
import {useWeatherDataStats} from "@/hooks/useWeatherDataStats.ts";


const LatestWeatherWidget = () => {
    const {data: latestData, isLoading: latestLoading, error: latestError} = useLatestWeatherData();
    const {data: statsData, isLoading: statsLoading, error: statsError} = useWeatherDataStats();

    if (latestLoading || statsLoading) return <div className="p-4 text-center">Loading weather data...</div>;
    if (latestError || statsError) {
        const errorMessage = latestError || statsError;
        return <div className="p-4 text-red-500">Error: {errorMessage}</div>;
    }
    if (!latestData || ! statsData) return <div className="p-4 text-center">No weather data available</div>;

    const latest_reading = latestData.latest_reading;
    const diff = latestData.difference_from_previous

    const formattingOptions: Intl.DateTimeFormatOptions = {
        timeZone: 'Europe/London',
        year: 'numeric',
        month: 'long',
        day: 'numeric',
        hour: 'numeric',
        minute: '2-digit',
        second: '2-digit',
        timeZoneName: 'short',
    };

    return (
        <div>
            <h2 className="text-2xl font-bold">Latest Matlock Weather Data</h2>
            <p className="text-slate-500 dark:text-slate-300 pb-3">
                Last Updated: {new Date(latestData.latest_reading.timestamp).toLocaleString("en-GB", formattingOptions)}
            </p>

        <div className="grid gap-4 md:grid-cols-3">
            <MetricCard label="Temperature" value={latest_reading.temperature} unit="°C" icon={<Thermometer />} change={diff?.temperature} stats={statsData.temperature} />
            <MetricCard label="Humidity" value={latest_reading.humidity} unit="%" icon={<Droplets />} change={diff?.humidity} stats={statsData.humidity} />
            <MetricCard label="Pressure" value={latest_reading.pressure} unit="hPa"  icon={<Gauge />} change={diff?.pressure} stats={statsData.pressure} />

       </div>
        </div>

    );
}

export default LatestWeatherWidget;