
const weatherConfigs = {
    temperature: {color: "text-orange-500", unit: "°C"},
    humidity: {color: "text-blue-500", unit: "%"},
    pressure: {color: "text-purple-500", unit: "hPa"}
};



function WeatherBlock({type, value, icon: Icon}) {
    const config = weatherConfigs[type];

        return (
        <div className="flex flex-col gap-1 justify-center items-center p-10 bg-gray-100 rounded-lg w-max">
            <div className="flex flex-row items-center gap-2">
                <Icon className={`w-5 h-5 ${config.color}`}/>
                <p className="capitalize">{type}</p>
            </div>
            <p>{Math.round(value)} {config.unit}</p>
        </div>
    )
}

export default WeatherBlock;