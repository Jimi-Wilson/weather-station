import WeatherBlock from "./WeatherBlock.jsx";
import {Thermometer, Gauge, Droplets} from "lucide-react";

function WeatherBlocks({weatherData}){

    const latestReading = weatherData.results[0];

    return (
        <div className="flex flex-col gap-10 flex-wrap sm:flex-row">
            <WeatherBlock type="temperature" value={latestReading.temperature} icon={Thermometer} />
            <WeatherBlock type="humidity" value={latestReading.humidity} icon={Droplets}/>
            <WeatherBlock type="pressure" value={latestReading.pressure} icon={Gauge}/>
        </div>
    )

}

export default WeatherBlocks;