import {useLatestWeatherData} from "@/hooks/useLatestWeatherData.ts";
import MetricCard from "@/components/MetricCard.tsx";
import {Thermometer, Gauge, Droplets} from "lucide-react";


const LatestWeatherWidget = () => {

    const {data, isLoading, error} = useLatestWeatherData();


    if (isLoading) return <div className="p-4 text-center">Loading weather data...</div>;
    if (error) return <div className="p-4 text-red-500">Error: {error}</div>;
    if (!data) return <div className="p-4 text-center">No weather data available</div>;

    const latest_reading = data.latest_reading;
    const diff = data.difference_from_previous

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
            <h2 className="text-2xl font-bold mb-2">Latest Weather Data</h2>
            <p className="text-slate-500 dark:text-slate-300 mb-4">
                Last Updated: {new Date(data.latest_reading.timestamp).toLocaleString("en-GB", formattingOptions)}
            </p>

        <div className="grid gap-4 md:grid-cols-3">
            <MetricCard label="Temperature" value={latest_reading.temperature} unit="°C" icon={<Thermometer />} change={diff?.temperature} />
            <MetricCard label="Humidity" value={latest_reading.humidity} unit="%" icon={<Droplets />} change={diff?.humidity} />
            <MetricCard label="Pressure" value={latest_reading.pressure} unit="hPa"  icon={<Gauge />} change={diff?.pressure}/>

       </div>
        </div>

    );



}

export default LatestWeatherWidget;