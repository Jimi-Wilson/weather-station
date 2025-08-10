import {useLatestWeatherData} from "@/hooks/useLatestWeatherData.ts";
import MetricCard from "@/components/MetricCard.tsx";
import {Thermometer, Gauge, Droplets} from "lucide-react";
import {useEffect, useRef, useState} from "react";
import type {WeatherReading} from "@/services/types.ts";

type SignificantChanges = {
    temperature: number | null;
    humidity: number | null;
    pressure: number | null;
};


const LatestWeatherWidget = () => {

    const {data, isLoading, error} = useLatestWeatherData();
    const previousData = useRef<WeatherReading | null>(null);
    const [changes, setChanges] = useState<SignificantChanges>({
        temperature: null,
        humidity: null,
        pressure: null,
    });

    useEffect(() => {
        if (previousData.current && data) {
            const change = {
                temperature: data?.temperature - previousData.current?.temperature,
                humidity: data?.humidity - previousData.current?.humidity,
                pressure: data?.pressure - previousData.current?.pressure,
            };

            const newChanges: SignificantChanges = {
                temperature: null,
                humidity: null,
                pressure: null,
            };

            if (Math.abs(change.temperature) > 0.5) {
                newChanges.temperature = change.temperature;
            }

            if (Math.abs(change.humidity) > 3) {
                newChanges.humidity = change.humidity;
            }

            if (Math.abs(change.pressure) > .25) {
                newChanges.pressure = change.pressure;
            }

            setChanges(newChanges);
        }
        previousData.current = data
    }, [data]);

    if (isLoading) return <div className="p-4 text-center">Loading weather data...</div>;
    if (error) return <div className="p-4 text-red-500">Error: {error}</div>;
    if (!data) return <div className="p-4 text-center">No weather data available</div>;


    return (
        <div>
            <h2 className="text-2xl font-bold mb-4">Latest Weather Data</h2>
            <p className="text-gray-600 mb-4">
                Last Updated: {new Date(data.timestamp).toLocaleString()}
            </p>

        <div className="grid gap-4 md:grid-cols-3">
            <MetricCard label="Temperature" value={data.temperature} unit="°C" icon={<Thermometer />} change={changes.temperature} />
            <MetricCard label="Humidity" value={data.humidity} unit="%" icon={<Droplets />} change={changes.humidity} />
            <MetricCard label="Pressure" value={data.pressure} unit="hPa"  icon={<Gauge />} change={changes.pressure}/>

       </div>
        </div>

    );



}

export default LatestWeatherWidget;