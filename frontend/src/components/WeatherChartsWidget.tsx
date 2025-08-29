import { useRecentWeatherData } from "@/hooks/useRecentWeatherData.ts";
import MetricChart from "@/components/MetricChart.tsx";
import {useMemo} from "react";



const WeatherChartsWidget = () => {

    const { data, isLoading, error } = useRecentWeatherData();

    const formattedData = useMemo(() =>
        data?.map(reading => ({
            ...reading,
            timestamp: new Date(reading.timestamp).toLocaleTimeString('en-GB', {
                hour: '2-digit',
                minute: '2-digit'
            }),
        })), [data]
    );



    if (error) return <div>Error: {error}</div>;

    if (isLoading) {
        return (
            <div className="mt-4">
                <h2 className="text-2xl font-bold mb-4">Weather Charts</h2>
                <div className={"flex flex-col gap-4"}>
                    {/* You can use your cool skeleton loaders here! */}
                    <MetricChart metric="temperature" isLoading={true} data={[]} />
                    <MetricChart metric="humidity" isLoading={true} data={[]} />
                    <MetricChart metric="pressure" isLoading={true} data={[]} />
                </div>
            </div>
        )
    }


    return (
        <div className="mt-4">
            <h2 className="text-2xl font-bold mb-4">Weather Charts</h2>


        <div className={"flex flex-col gap-4"}>
            <MetricChart metric="temperature" data={formattedData || []} />
            <MetricChart metric="humidity" data={formattedData || []} />
            <MetricChart metric="pressure" data={formattedData || []} />

        </div>

        </div>
    )
}


export default WeatherChartsWidget