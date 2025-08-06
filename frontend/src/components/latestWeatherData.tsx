import { useLatestWeatherData } from "@/hooks/useLatestWeatherData.ts";
import {Button} from "@/components/ui/button.tsx";

const CurrentWeatherWidget = () => {
    const { data, isLoading, error, fetchData} = useLatestWeatherData();

    if (isLoading) {
        return <div>Loading...</div>;
    }

    if (error) {
        return <div>Error: {error}</div>;
    }

    return (
        <div>
            <h2>Current Weather</h2>
            {data ? (
                <>
                    <p>Temperature: {data.temperature}°C</p>
                    <p>Humidity: {data.humidity}%</p>
                    <p>Last updated: {new Date(data.timestamp).toLocaleTimeString()}</p>
                </>
            ) : (
                <p>No weather data available.</p>
            )}
            <Button onClick={fetchData}>Refresh</Button>
        </div>
    );

};

export default CurrentWeatherWidget;